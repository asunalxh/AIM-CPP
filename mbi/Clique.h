
#ifndef AIM_CPP_CLIQUE_H
#define AIM_CPP_CLIQUE_H

#include "../util/common.h"
#include <vector>
#include <iostream>


class Clique {
    std::vector<Attribute> attrs;
public:
    Clique(std::vector<Attribute> attrs = {});

    int index(const Attribute &k) const;

    bool contains(const Attribute &key) const;

    int size() const;

    std::vector<Attribute> getAttrList() const;

    bool isSubsetOf(const Clique &clique) const;

//    bool isSameSetOf(const Clique &clique) const;

    Attribute at(const int &i) const;

    Attribute &operator[](const int &i);

    Clique &operator|=(const Clique &clique);

    Clique operator|(const Clique &clique) const;

    Clique operator&(const Clique &clique) const;

    Clique &operator=(const Clique &clique);


    bool operator==(const Clique &clique) const;

    bool operator!=(const Clique &clique) const;

    bool operator<(const Clique &clique) const;

    Clique &operator-=(const Clique &clique);

    void remove(const Attribute& attr);

    void add(const Attribute& attr);

    Clique intersection(const Clique& clique) const ;

    friend std::ostream& operator<<(std::ostream& os,const Clique& clique);

    std::vector<Attribute>::iterator begin();

    std::vector<Attribute>::iterator end();

};


#endif //AIM_CPP_CLIQUE_H
