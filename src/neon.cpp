/*

MIT License

Copyright (c) 2017 Noah Andreas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#include "neon.h"
#include "xdgmenu.h"
#include <QDebug>


namespace mlde {


Neon::Neon(QString menuFile)
{

    loadMenuFile(&root, menuFile);

    processTags(&root);

    simplify();
    mergeMenus();
    moveMenus();
    mergeMenus();
    deleteDeletedMenus();
    processDirectoryEntries(QStringList());
    processDesktopEntries();
    processLayouts();
    deleteEmpty();
    fixSeparators();
}


void
Neon::loadMenuFile(XdgTreeNode* node, const QString& menuFile)
{

    // check parameter
    if (menuFile.isEmpty())
    {
        qCritical() << "mlde::neon::XdgXml::loadMenuFile() parameter menuFile is empty";
        return;
    }

    qDebug() << "mlde::neon::XdgXml::loadMenuFile() " << menuFile;


    // check file
    QFile *file = new QFile(menuFile);

    if ( !file->exists() )
    {
        qCritical() << "mlde::neon::XdgXml::loadMenuFile() " << menuFile << " file does not exists";
        return;
    }

    if ( !file->open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qCritical() << "mlde::neon::XdgXml::loadMenuFile() " << menuFile << " unable to open file";
        return;
    }


    // add watcher so we will be informed of changes

    //addWatcher(MenuFile);


    // set basedir for this file to resolve relative file-paths in the menufile

    QString baseDir = QFileInfo(menuFile).canonicalPath();


    // create reader and parse the file
    // create nodes in the tree for each tag

    QXmlStreamReader xml;
    xml.setDevice(file);

    while (!xml.atEnd())
    {
        xml.readNext();


        if ( xml.isStartElement())
        {
            // create new tree node and save properties

            XdgTreeNode* n = new XdgTreeNode();

            n->name = xml.name().toString();
            //n->attributes = xml.attributes();

            // add new node to child of current node
            node->childs.append(n);

            // set current-pointer to new node
            node = n;
        }


        else if ( xml.isCharacters() && !xml.isWhitespace() )
        {
            // text between start and endtag, save to currentnode
            node->text = xml.text().toString();
        }


        else if ( xml.isEndElement())
        {
            // go back to parent node - if there is no parent, there are more endtags than starting tags, duh

            if ( !node->parent )
            {
                qCritical() << "mlde::neon::XdgXml::loadMenuFile()  EndElement: error going up to parent; parent is null";
                return;
            }

            node = node->parent;
        }


    } // while

}


void
Neon::processTags(XdgTreeNode* node)
{
    // check all children if further processing is necessary

    foreach (XdgTreeNode* n, node->childs)
    {
        if (n->name == "MergeFile")
        {
            processMergeFile(n);
        }

        else if (n->name == "MergeDir")
        {
            processMergeDir(n);
        }

        else if (n->name == "DefaultMergeDirs")
        {
            processDefaultMergeDirs(n);
        }

        else if (n->name == "DefaultAppDirs")
        {
            processDefaultAppDirs(n);
        }

        else if (n->name == "DefaultDirectoryDirs")
        {
            processDefaultDirectoryDirs(n);
        }

        else if (n->name == "Menu")
        {
            processNodes(n);
        }

    } // foreach

}


void
Neon::processMergeFile(XdgTreeNode* node)
{
    qDebug() << "mlde::neon::XdgXml::processMergeFile() " << node->text;


    // check if file is not already loaded!


    // load xml file, this will create nodes as childs of the current node
    loadMenuFile(node, node->text);


    // 1. find <menu>

    XdgTreeNode* n = node->firstChild;

    while (n)
    {
        if (n->name == "Menu")
            break;
        n = n->next;
    }

    // 2. move all childs of <menu> to same level as <mergefile> (because they should replace <mergefile>!)

    n = n->firstChild;

    XdgTreeNode* next;

    while (n)
    {
        next = n->next;

        // ignore <name> tag
        if (n->name != "Name")
            node->insertAfter(n);

        n = next;
    }

    // 3. remove <mergefile>

    node->remove();

}


void
Neon::processMergeDir(XdgTreeNode* node)
{
    qDebug() << "mlde::neon::XdgXml::processMergeDir() " << node->text;

    // replace <MergeDir> with <MergeFile>-tags for each *.menu-file in <MergeDir>
}


void
Neon::processDefaultMergeDirs(XdgTreeNode* node)
{
    qDebug() << "mlde::neon::XdgXml::processDefaultMergeDirs()";

    // replace <DefaultMergeDirs> with <MergeDir>-tags for each dir from environment-var

    QStringList dirs = XdgDirs::xdgDataDirs();
    dirs.prepend(XdgDirs::xdgDataHome());
    
    foreach (const QString &dir, dirs)
    {
        XdgTreeNode* n = new XdgTreeNode();

        n->name = "MergeDir";
        n->text = dir;

        node->insertAfter(n);

        qDebug() << "mlde::neon::XdgXml::processDefaultMergeDirs() added " << n->text;
    }

    node->remove();
}


void
Neon::processDefaultAppDirs(XdgTreeNode* node)
{
    qDebug() << "mlde::neon::XdgXml::processDefaultAppDirs()";

    // replace <DefaultAppDirs> with <AppDir>-tags for each dir from environment-var

    QStringList dirs = XdgDirs::xdgDataDirs();
    dirs.prepend(XdgDirs::xdgDataHome());
    
    foreach (const QString &dir, dirs)
    {
        XdgTreeNode* n = new XdgTreeNode();

        n->name = "AppDir";
        n->text = dir + QLatin1String("/applications/");

        node->insertAfter(n);

        qDebug() << "mlde::neon::XdgXml::processDefaultAppDirs() added " << n->text;
    }

    node->remove();
}


void
Neon::processDefaultDirectoryDirs(XdgTreeNode* node)
{
    qDebug() << "mlde::neon::XdgXml::processDefaultDirectoryDirs()";

    // replace <DefaultDirectoryDirs> with <DirectoryDir>-tags for each dir from environment-var

    QStringList dirs = XdgDirs::xdgDataDirs();
    dirs.prepend(XdgDirs::xdgDataHome());

    foreach (const QString &dir, dirs)
    {
        XdgTreeNode* n = new XdgTreeNode();

        n->name = "DirectoryDir";
        n->text = dir + QLatin1String("/desktop-directories/");

        node->insertAfter(n);

        qDebug() << "mlde::neon::XdgXml::processDefaultDirectoryDirs() added " << n->text;
    }

    node->remove();
}


void 
Neon::simplify()
{
}


void 
Neon::mergeMenus()
{
}


void 
Neon::moveMenus()
{
}


void 
Neon::deleteDeletedMenus()
{
}


void 
Neon::processDirectoryEntries(QStringList())
{
}


void 
Neon::processDesktopEntries()
{
}


void 
Neon::processLayouts()
{
}


void 
Neon::deleteEmpty()
{
}


void 
Neon::fixSeparators()
{
}


} // namespace
