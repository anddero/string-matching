//
// Created by andde on 11-Jan-21.
//

#ifndef FINDSONGDUPLICATES_OPERATION_H
#define FINDSONGDUPLICATES_OPERATION_H


struct Operation {
    enum Type {
        DELETE,
        INSERT,
        REPLACE
    };

    Type type;
    unsigned long long index_one;
    unsigned long long index_two;
    unsigned long long length;

public:
    Operation(Type type, unsigned long long int index_one, unsigned long long int index_two, unsigned long long int length);

    bool operator==(const Operation &rhs) const;

    bool operator!=(const Operation &rhs) const;
};


#endif //FINDSONGDUPLICATES_OPERATION_H
