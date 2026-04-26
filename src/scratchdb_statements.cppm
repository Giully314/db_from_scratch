// PURPOSE:
//
// CLASSES:
// 
// DESCRIPTION:
// 

export module scratchdb.statements;

import std;

import scratchdb.row;

namespace scratchdb {


export struct SelectStatement {

};

export struct InsertStatement {
    Row row;
};

export using Statement = std::variant<SelectStatement, InsertStatement>;

    
} // namespace scratchdb
