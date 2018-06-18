#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <string>
#include <utility>
#include <map>
#include "Entity.hpp"

namespace hlt {

/** Representation of a Halite player. */
struct Player {
    friend class PlayerFactory;

    /** Type of a location . */
    using Location = std::pair<dimension_type, dimension_type>;

    /** Type of the Entity list of a player. */
    using Entities = std::map<Location, std::shared_ptr<Entity>> ;

    id_type player_id;            /**< The unique ID of the player. */
    std::string name;             /**< The name of the player. */
    std::string command;          /**< The bot command for the player. */
    energy_type energy{};         /**< The amount of energy stockpiled by the player. */
    Location factory_location{};  /**< The factory location of the player. */
    Entities entities;            /**< Mapping of location of entity to entity shared ptr */

    /**
     * Convert a Player to JSON format.
     * @param[out] json The output JSON.
     * @param player The Player to convert.
     */
    friend void to_json(nlohmann::json &json, const Player &player);

    /** Test two Player instances for equality. */
    bool operator==(const Player &other) const { return player_id == other.player_id; }

    /** Order two Player instances by ID. */
    bool operator<(const Player &other) const { return player_id < other.player_id; }

    /**
     * Write a Player to bot serial format.
     * @param ostream The output stream.
     * @param player The Player to write.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &ostream, const Player &player);

    /** Default constructor required by std::map. */
    Player() = default;

private:
    /**
     * Construct Player from ID, name, and command.
     * @param player_id The player ID.
     * @param name The player name.
     * @param command The player bot command.
     */
    Player(id_type player_id, std::string name, std::string command) :
            player_id(player_id), name(std::move(name)), command(std::move(command)) {}

    /**
     * Construct Player from ID, name, energy, factory location, and entities.
     * @param player_id The player ID.
     * @param name The player name.
     * @param energy The energy.
     * @param factory_location The factory location.
     * @param entities The location -> entity mapping.
     */
    Player(id_type player_id, std::string name, energy_type energy,
           Location factory_location, Player::Entities entities)
            : player_id(player_id), name(std::move(name)), energy(energy), factory_location(std::move(factory_location)),
              entities(std::move(entities)) {}
};

/**
 * Write a list of Players to bot serial format.
 * @param ostream The output stream.
 * @param players The Players to write.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &ostream, const std::list<Player> &players);

/** Factory which produces Players. */
class PlayerFactory {
    /** The next player to allocate, starting from zero. */
    id_type next_player{};

public:
    /**
     * Make a new player with default name (which is ID), and a command.
     * @param command The bot command.
     * @return The new player.
     */
    Player new_player(std::string command) {
        return {next_player++, std::to_string(next_player), std::move(command)};
    }

    PlayerFactory() = default;
};

}

namespace std {
template<>
struct hash<hlt::Player> {
    size_t operator()(const hlt::Player &player) const {
        return (size_t) player.player_id;
    }
};
}
#endif /* PLAYER_H */