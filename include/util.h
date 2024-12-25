#include <concepts>
template <typename KeyType, typename MapType>
concept KeyMatchesMap = requires(const MapType &map, const KeyType &key) {
    { map.find(key) } -> std::same_as<typename MapType::const_iterator>;
};

namespace util {

template <typename KeyType, typename MapType>
    requires KeyMatchesMap<KeyType, MapType>
inline auto isValidKey(const KeyType &key, const MapType &map) -> bool {
    return map.find(key) != map.end();
}

} // namespace util
