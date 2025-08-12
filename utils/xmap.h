#ifndef XMAP_H
#define XMAP_H

#include <cstdint>
#include <iostream>
#include <map>


/**
 * @class yahangsu
 * @brief 
 * 
 * 具备唯一ID的映射容器类，支持插入、删除、更新键值对等操作;支持返回最小可用 ID。
 * 
 * @note 
 */

namespace nameless {

    template<typename  T>
    class XMap {
    public:
        bool insert(T value) {
            uint16_t id = find_min_available_id();
            if (id == 0) return false; // 已满
            data_[id] = std::move(value);
            return true;
        }

        bool erase(uint16_t id) {
            return data_.erase(id) > 0;
        }

        bool updateKey(uint16_t oldKey, uint16_t newKey) {

            auto it = data_.find(oldKey);
            if (it == data_.end()) return false; // 如果没有找到 oldKey，返回 false

            // 检查新的 ID 是否已存在
            if (data_.contains(newKey)) return false; // 新 ID 已经存在，返回 false  --> 这里要看编译器版本

            // 获取当前的 value
            T value = std::move(it->second);

            // 删除旧的 ID
            data_.erase(it);

            // 插入新的 ID 和原来的 value
            data_[newKey] = std::move(value);
            return true;
        }

        [[nodiscard]] uint16_t find_min_available_id() const {
            uint16_t expected = 1;
            for (auto &[id, _] : data_) {
                if (id != expected) return expected;
                ++expected;
            }
            return (expected <= max_id_) ? expected : 0; // 0 表示没有可用 ID
        }

        void print() const {
            for (auto &[id, val] : data_) {
                std::cout << id << " => " << val << "\n";
            }
        }

    private:
        static constexpr uint16_t max_id_ = 65535;
        std::map<uint16_t, T> data_;
    };


}

#endif //XMAP_H