    /* freg, Free-Roaming Elementary Game with open and interactive world
    *  Copyright (C) 2012-2015 Alexander 'mmaulwurff' Kromm
    *  mmaulwurff@gmail.com
    *
    * This file is part of FREG.
    *
    * FREG is free software: you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation, either version 3 of the License, or
    * (at your option) any later version.
    *
    * FREG is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with FREG. If not, see <http://www.gnu.org/licenses/>. */

#include "WaysTreeBuilder.h"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>

int WaysTreeBuilder::currentIndex = 0;
const char WaysTreeBuilder::file_name[] = "WaysTreeData.cpp";

void WaysTreeBuilder::operator+=(const WaysTreeBuilder* const new_chain) {
    if ( new_chain->nexts.empty() ) {
        delete new_chain;
        return;
    }
    const auto next_node_index = std::distance(
            std::cbegin(nexts),
            std::find_if( nexts.cbegin()
                        , nexts.cend()
                        , [&new_chain](const WaysTreeBuilder* const node)
    {
        return (*node == *new_chain->nexts[0]);
    }));
    if ( next_node_index < nexts.size() ) { // found
        *nexts[next_node_index] += new_chain->nexts[0];
    } else {
        nexts.push_back(new_chain->nexts[0]);
    }
    delete new_chain;
}

bool WaysTreeBuilder::operator==(const WaysTreeBuilder & other) const {
    return other.x == x
        && other.y == y
        && other.z == z;
}

WaysTreeBuilder::WaysTreeBuilder()
    : x(0)
    , y(0)
    , z(0)
    , nexts()
{
    const int MAX = 4;
    for (int x=-MAX; x<=MAX; ++x)
    for (int y=-MAX; y<=MAX; ++y)
    for (int z=-MAX; z<=MAX; ++z) {
        if ( std::abs(x) != MAX
          && std::abs(y) != MAX
          && std::abs(z) != MAX ) continue;

        WaysTreeBuilder* const root = new WaysTreeBuilder(0, 0, 0); // new chain
        WaysTreeBuilder* tail = root;

        int i=0, j=0, k=0;
        while ( not (i==x*MAX && j==y*MAX && k==z*MAX) ) {
            i += x;
            j += y;
            k += z;
            tail->nexts.push_back(new WaysTreeBuilder(i / MAX, j / MAX, k / MAX));
            tail = tail->nexts[0];
        }

        *this += root;
    }

    SetIndices();
    PrintBranches();
}

WaysTreeBuilder::WaysTreeBuilder(const int _x, const int _y, const int _z)
    : x(_x)
    , y(_y)
    , z(_z)
    , nexts()
{}

void WaysTreeBuilder::Clear() const {
    for (WaysTreeBuilder* const branch : nexts) {
        branch->Clear();
        delete branch;
    }
}

void WaysTreeBuilder::SetIndices() {
    index = currentIndex;
    currentIndex += nexts.size() + 4;

    for (WaysTreeBuilder* const branch : nexts) {
        branch->SetIndices();
    }
}

void WaysTreeBuilder::PrintBranches() {
    static const char file_name[] = "WaysTreeData.cpp";
    using namespace std;
    { ofstream reset_file_stream(file_name); }

    ofstream stream(file_name, ios::app);
    stream << "// This file is generated by WaysTreeBuilder." << endl
           << "// Do not edit." << endl << endl
           << "// x, y, z, branch_count, branch_indices" << endl
           << "const int16_t WaysTree::data[] {" << endl;
    PrintBranchesRecursive();
    cout << "Result array size: " << currentIndex;
    stream << "};" << endl;
}

void WaysTreeBuilder::PrintBranchesRecursive() {
    std::ofstream stream(file_name, std::ios::app);
    static const char comma[] = ", ";
    stream << x << comma << y << comma << z << comma
           << nexts.size() << comma;
    for (WaysTreeBuilder* const branch : nexts) {
        stream << branch->index << comma;
    }
    stream << std::endl;

    for (WaysTreeBuilder* const branch : nexts) {
        branch->PrintBranchesRecursive();
    }
}
