#pragma once 

#include "types.h"
#include "file.h"

class CsvFile : public File
{
public:
    DELETE_DEFAULT_CONSTRUCTOR(CsvFile);

    CsvFile(const String& filePath, const Vector<String>& columnNames)
        : File(filePath)
        , _columnNames(columnNames)
    {
    }

    template <class... Args>
    Result<> AppendRow()
    {
        if(sizeof...(Args) != _columnNames.size())
            return { ErrorCode::ColumnCountMismatch };




        return Success;
    }

    inline const Vector<String>& GetColumnsName() const
    {
        return _columnNames;
    }

    Vector<String> _columnNames;
    Vector<Vector<String>> _data;
};


