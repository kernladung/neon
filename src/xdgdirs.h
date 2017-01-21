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


#include <QString>
#include <QStringList>


namespace mlde {
namespace neon {


static class XdgDirs
{

public:

    // $XDG_DATA_HOME defines the base directory relative to which user specific data files
    // should be stored. If $XDG_DATA_HOME is either not set or empty, a default equal to
    // $HOME/.local/share should be used.

    static QString xdgDataHome();


    // $XDG_CONFIG_HOME defines the base directory relative to which user specific configuration
    // files should be stored. If $XDG_CONFIG_HOME is either not set or empty, a default equal
    // to $HOME/.config should be used.

    static QString xdgConfigHome();


    // $XDG_DATA_DIRS defines the preference-ordered set of base directories to search for data
    // files in addition to the $XDG_DATA_HOME base directory. If $XDG_DATA_DIRS is either not set
    // or empty, a value equal to /usr/local/share:/usr/share is used.

    static QStringList xdgDataDirs();


    // $XDG_CONFIG_DIRS defines the preference-ordered set of base directories to search for
    // configuration files in addition to the $XDG_CONFIG_HOME base directory. If $XDG_CONFIG_DIRS
    // is either not set or empty, a value equal to /etc/xdg should be used.

    static QStringList xdgConfigDirs();


private:

    // Helper functions
    static void fixBashShortcuts(QString &s);
    static void removeEndingSlash(QString &s);
    static void clean(QStringList &dirs);


};


}} // namespace


