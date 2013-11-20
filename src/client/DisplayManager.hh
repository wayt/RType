#ifndef DISPLAYMANAGER_H_
# define DISPLAYMANAGER_H_

# ifndef __DWIDTH
#  define __DWIDTH 1920
#  define __DHEIGHT 1200
# endif

# include <iostream>
# include <SFML/Graphics.hpp>
# include <Core/Core.hh>
#include "Thread.hpp"
#include "Mutex.hpp"
#include "APlayer.hh"

enum DisplayMode
{
    MODE_MENU       = 0,
    MODE_GAME       = 1,
};

class Client;

class DisplayManager
{
    public:
        DisplayManager(Client* client, int width = __DWIDTH, int height = __DHEIGHT, bool type = false);
        ~DisplayManager();
        void gameMode();
        void menuMode();

        void Start(DisplayMode mode = MODE_MENU);
        void SwitchMode(DisplayMode mode);
        bool IsAlive() const { return _alive; }
        inline void flushAll() { _engine->flushScene(); _engine->flushEvent(); }
        inline void UpdateAlive() { _alive = DamnCute::Core::getInstance()->getWindowStatus(); }
        void Init();

        void AddPlayer(DamnCute::APlayer* player, uint32 id);
        DamnCute::APlayer* GetPlayer(uint32 id);
        DamnCute::APlayer const* GetPlayer(uint32 id) const;

    private:
        DamnCute::Core* _engine;
        bool _alive;
        DisplayMode _mode;
        bool _init;
        int32 _width;
        int32 _height;
        bool _fullscreen;
        Client* _client;
        std::map<uint32, DamnCute::APlayer*> _players;
};

#endif /* !DISPLAYMANAGER_H_ */
