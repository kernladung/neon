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


#include "node.h"
#include <QDebug>


namespace kernladung {
namespace neon {


Node::Node()
{
}


Node::~Node()
{
    qDeleteAll(childs);
    childs.clear();
}

/*
void
Node::appendChild(Node* child)
{
    child->parent = this;

    if (firstChild == 0)
    {
        firstChild = child;
    }
    else
    {
        lastChild->next = child;
        child->previous = lastChild;
    }

    lastChild = child;
}


void
Node::insertAfter(Node* node)
{
    node->parent = parent;

    // when there is a next node, insert new node before
    if (next)
    {
        next->previous = node;
        node->next = next;
    }

    // insert new node after this node
    next = node;
    node->previous = this;
}


void
Node::remove()
{
    if (previous)
        previous->next = next;

    if (next)
        next->previous = previous;

    delete this;
}
*/

}} // namespace
