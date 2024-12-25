#include <cstddef>
#include <functional>
enum class Mnemonic {
    ADD,
    MOV,
    SUB,
    JUMP,
};

// Specialize std::hash for Mnemonic
namespace std {
template <> struct hash<Mnemonic> {
    size_t operator()(const Mnemonic &mnemonic) const {
        // Combine hash values of members
        return static_cast<std::size_t>(mnemonic);
    }
};
} // namespace std
