// SPDX-FileCopyrightText: 2022 Contributors to the Power Grid Model project <dynamic.grid.calculation@alliander.com>
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#ifndef POWER_GRID_MODEL_META_DATA_GEN_HPP
#define POWER_GRID_MODEL_META_DATA_GEN_HPP

#include <map>
#include <string>

#include "../all_components.hpp"
#include "../power_grid_model.hpp"
#include "input.hpp"
#include "meta_data.hpp"
#include "output.hpp"

// generate of meta data
namespace power_grid_model {

namespace meta_data {

template <class T>
struct MetaDataGeneratorImpl;

template <class... ComponentType>
struct MetaDataGeneratorImpl<ComponentList<ComponentType...>> {
    MetaDataGeneratorImpl() {
        static constexpr std::array func_arr{&retrieve_single_type<ComponentType>...};
        for (auto const func : func_arr) {
            func(*this);
        }
    }

    template <class Component>
    static void retrieve_single_type(MetaDataGeneratorImpl<ComponentList<ComponentType...>>& inst) {
        inst.meta_data["input"][Component::name] = get_meta<typename Component::InputType>{}();
        inst.meta_data["update"][Component::name] = get_meta<typename Component::UpdateType>{}();
        inst.meta_data["sym_output"][Component::name] = get_meta<typename Component::template OutputType<true>>{}();
        inst.meta_data["asym_output"][Component::name] = get_meta<typename Component::template OutputType<false>>{}();
    }

    AllPowerGridMetaData meta_data;
};

using MetaDataGenerator = MetaDataGeneratorImpl<AllComponents>;

inline AllPowerGridMetaData const& meta_data() {
    static MetaDataGenerator const meta{};
    return meta.meta_data;
}

}  // namespace meta_data

}  // namespace power_grid_model

#endif
