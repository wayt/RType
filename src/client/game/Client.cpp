#include "Client.h"
#include "Utils.hpp"
#include "Opcodes.h"

#include <iostream>

Client::Client() : _service(), _udpSocket(this, _service), _clientKey(0),
    _recvQueue(), _display(NULL), _player(NULL)
{
}

Client::~Client()
{
    delete _display;
}

void Client::Initialize(int32 width, int32 height, bool fullscreen)
{
    _display = new DisplayManager(this, width, height, fullscreen);
}

void Client::Launch(uint32 clientId)
{
    _service.Start();
    InitializeGame(clientId, "127.0.0.1", "5000"); 
    _display->SwitchMode(MODE_GAME);
    _display->Init();

    while (_display->IsAlive())
    {
        _display->Init(); // Init only if needed
        _display->flushAll();
        _display->UpdateAlive();

        UpdateIncomingPackets();
        UpdatePlayerPosition();
    }
    _service.Stop();
    _service.Wait();
}

void Client::UpdateIncomingPackets()
{
    uint32 size = _recvQueue.size();
    for (uint32 i = 0; i < size && !_recvQueue.empty(); ++i)
    {
        Packet* pkt = _recvQueue.get();
        if (!pkt)
            continue;

        Opcode::OpcodeHandler const* opcodehandler = Opcode::GetHandler((Opcodes)pkt->GetOpcode());
        if (!opcodehandler)
        {
            std::cerr << "Error: receiv an unknow opcode : " << pkt->GetOpcode() << std::endl;
            delete pkt;
            continue;
        }
        if (opcodehandler->handler)
            (this->*(opcodehandler->handler))(pkt);

        delete pkt;
    }
}

void Client::UDPHandleReceive(Packet const* recvPkt)
{
    if (recvPkt->GetOpcode() == SMSG_PING)
    {
        Packet pkt(CMSG_PONG);
        UDPSend(pkt);
        return;
    }
    Packet* recv = new Packet(*recvPkt);
    _recvQueue.add(recv);
}

bool Client::InitializeGame(uint32 clientKey, std::string const& addr, std::string const& port)
{
    if (!_udpSocket.Connect(addr, port))
        return false;

    _clientKey = clientKey;
    return true;
}

void Client::UDPSend(Packet const& pkt)
{
    _udpSocket.Send(pkt.data(), pkt.size());
}

void Client::UpdatePlayerPosition()
{
    static sf::Vector2f pos(0.0f, 0.0f);

    if (_player)
    {
        sf::Sprite const& sprite = _player->getPlayer();
        sf::Vector2f const& newPos = sprite.getPosition();
        if (newPos != pos)
        {
            pos = newPos;
            Packet pkt(CMSG_PLAYER_POSITION);
            pkt << pos.x;
            pkt << pos.y;
            UDPSend(pkt);
        }
    }
}

