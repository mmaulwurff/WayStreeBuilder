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

#ifndef WAYS_TREE_H
#define WAYS_TREE_H

#include <vector>

class WaysTreeBuilder {
public:
    WaysTreeBuilder();

    void Clear() const;

private:
    WaysTreeBuilder(int x, int y, int z);

    void operator+=(const WaysTreeBuilder* new_chain);
    bool operator==(const WaysTreeBuilder&) const;

    void SetIndices();

    void PrintBranches();
    void PrintBranchesRecursive();

    std::vector<WaysTreeBuilder*> nexts;

    int index;
    static int currentIndex;

    const int x, y, z;

    static const char file_name[];
};

#endif // WAYS_TREE_H
