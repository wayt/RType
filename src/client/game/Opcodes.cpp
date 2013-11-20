#include "Opcodes.h"
#include "Client.h"
#include "Packet.hpp"

Opcode::OpcodeHandler const* Opcode::GetHandler(Opcodes op)
{
    static OpcodeHandler handlers[] = {
        { SMSG_GREETING, &Client::HandleGreeting            },
        { SMSG_CONNECT_RESULT, &Client::HandleConnectResult },
        { SMSG_PLAYER_POSITION, &Client::HandlePlayerPosition },
        { SMSG_ADD_PLAYER, &Client::HandleAddPlayer         },
        { SMSG_REMOVE_PLAYER, &Client::HandleRemovePlayer },
        { SMSG_SHOT, &Client::HandlePlayerShot              },
        { OPCODE_COUNT, NULL                                }

    };

    for (uint32 i = 0; handlers[i].op != OPCODE_COUNT; ++i)
        if (handlers[i].op == op)
            return &handlers[i];
    return NULL;
}
