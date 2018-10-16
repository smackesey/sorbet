#ifndef RUBY_TYPER_NAMEREF_H
#define RUBY_TYPER_NAMEREF_H
#include "common/common.h"
#include "core/DebugOnlyCheck.h"
namespace sorbet::core {
class GlobalState;
class GlobalSubstitution;
class Name;

struct NameDataDebugCheck {
    const GlobalState &gs;
    const unsigned int nameCountAtCreation;

    NameDataDebugCheck(const GlobalState &gs);
    void check() const;
};

/** This is to `NameRef &` what SymbolData is to `SymbolRef &`. Read docs on SymbolData */
class NameData : private DebugOnlyCheck<NameDataDebugCheck> {
    Name &name;

public:
    NameData(Name &ref, const GlobalState &gs);
    Name *operator->();
    const Name *operator->() const;
};
constexpr size_t sizeof__Name = sizeof(Name *);
constexpr size_t alignof__Name = alignof(Name *);
CheckSize(NameData, sizeof__Name, alignof__Name);

struct NameRefDebugCheck {
    int globalStateId;
    int parentGlobalStateId;

    constexpr NameRefDebugCheck() : globalStateId(-1), parentGlobalStateId(-1) {}

    NameRefDebugCheck(int globalStateId, int parentGlobalStateId)
        : globalStateId(globalStateId), parentGlobalStateId(parentGlobalStateId) {}

    void check(const GlobalState &gs, int id) const;
    void check(const GlobalSubstitution &subst) const;
};

class NameRef final : private DebugOnlyCheck<NameRefDebugCheck> {
public:
    friend GlobalState;
    friend Name;

    NameRef() : _id(-1){};

    // WellKnown is a tag to statically indicate that the caller is deliberately
    // constructing a well-known name, whose ID is stable across all
    // GlobalStates. This should never be used outside of the name constructors
    // generated by tools/generate_names.cc
    struct WellKnown {};

    constexpr NameRef(WellKnown, unsigned int id) : _id(id) {}

    NameRef(const GlobalState &gs, unsigned int id);

    NameRef(const NameRef &nm) = default;

    NameRef(NameRef &&nm) = default;

    NameRef &operator=(const NameRef &rhs) = default;

    bool operator==(const NameRef &rhs) const {
        return _id == rhs._id;
    }

    bool operator!=(const NameRef &rhs) const {
        return !(rhs == *this);
    }

    inline int id() const {
        return _id;
    }

    NameData data(GlobalState &gs) const;

    const NameData data(const GlobalState &gs) const;

    // Returns the `0` NameRef, used to indicate non-existence of a name
    static NameRef noName() {
        return NameRef(WellKnown{}, 0);
    }

    inline bool exists() const {
        return _id != 0;
    }

    NameRef addEq(GlobalState &gs) const;

    NameRef addAt(GlobalState &gs) const;

    NameRef prepend(GlobalState &gs, std::string_view s) const;

    std::string toString(const GlobalState &gs) const;

    std::string show(const GlobalState &gs) const;

    void enforceCorrectGlobalState(const GlobalState &gs) const;
    void sanityCheckSubstitution(const GlobalSubstitution &subst) const;

public:
    int _id;
};

CheckSize(NameRef, 4, 4);
} // namespace sorbet::core

template <> struct std::hash<sorbet::core::NameRef> {
    size_t operator()(const sorbet::core::NameRef &x) const {
        return x._id;
    }
};

template <> struct std::equal_to<sorbet::core::NameRef> {
    constexpr bool operator()(const sorbet::core::NameRef &lhs, const sorbet::core::NameRef &rhs) const {
        return lhs._id == rhs._id;
    }
};
#endif // RUBY_TYPER_NAMEREF_H
