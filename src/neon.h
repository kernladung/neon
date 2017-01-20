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


#pragma once


#include "xdgtree.h"


namespace kernladung {


class Neon
{

public:
    explicit Neon(QString fileName);
    
private:
    neon::XdgTreeNode root;

    void loadMenuFile(XdgTreeNode* node, const QString& menuFile);
    void processNodes(XdgTreeNode* node);
    void processMergeFile(XdgTreeNode* node);
    void processMergeDir(XdgTreeNode* node);
    void processDefaultMergeDirs(XdgTreeNode* node);
    void processDefaultAppDirs(XdgTreeNode* node);
    void processDefaultDirectoryDirs(XdgTreeNode* node);

    void simplify();
    void mergeMenus();
    void moveMenus();
    void deleteDeletedMenus();
    void processDirectoryEntries(QStringList());
    void processDesktopEntries();
    void processLayouts();
    void deleteEmpty();
    void fixSeparators();

};


} // namespace



