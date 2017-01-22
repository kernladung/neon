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


#include "xdgdirs.h"
#include <stdlib.h>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>


namespace mlde {
namespace hafnium {


void
XdgDirs::fixBashShortcuts(QString &s)
{
    if (s.startsWith(QLatin1Char('~')))
        s = QFile::decodeName(qgetenv("HOME")) + (s).mid(1);
}


void
XdgDirs::removeEndingSlash(QString &s)
{
    if (s.length() > 1 && s.endsWith(QLatin1Char('/')))
        s.chop(1);
}


void
XdgDirs::clean(QStringList &dirs)
{
    const int N = dirs.count();
    for(int i = 0; i < N; ++i)
    {
        fixBashShortcuts(dirs[i]);
        removeEndingSlash(dirs[i]);
    }
}


QString
XdgDirs::xdgDataHome()
{
    QString s = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    fixBashShortcuts(s);
    removeEndingSlash(s);
    return s;
}


QString XdgDirs::xdgConfigHome()
{
    QString s = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    fixBashShortcuts(s);
    removeEndingSlash(s);
    return s;
}


QStringList XdgDirs::xdgDataDirs()
{
    QString d = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    QStringList dirs = d.split(QLatin1Char(':'), QString::SkipEmptyParts);

    if (dirs.isEmpty())
    {
        dirs.append(QString::fromLatin1("/usr/local/share"));
        dirs.append(QString::fromLatin1("/usr/share"));
    }
    else
    {
        QMutableListIterator<QString> it(dirs);
        while (it.hasNext())
        {
            const QString dir = it.next();
            if (!dir.startsWith(QLatin1Char('/')))
                it.remove();
        }
    }

    dirs.removeDuplicates();
    clean(dirs);
    return dirs;
}


QStringList XdgDirs::xdgConfigDirs()
{
    QStringList dirs;
    const QString env = QFile::decodeName(qgetenv("XDG_CONFIG_DIRS"));
    if (env.isEmpty())
        dirs.append(QString::fromLatin1("/etc/xdg"));
    else
        dirs = env.split(QLatin1Char(':'), QString::SkipEmptyParts);

    clean(dirs);
    return dirs;
}


}} // namespace
