﻿#pragma once

#include "Poller.h"
#include <map>
#include "../base/Platform.h"

namespace net
{
    class EventLoop;
    class Channel;

    class SelectPoller : public Poller
    {
    public:
        SelectPoller(EventLoop* loop);
        virtual ~SelectPoller();

        virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels);
        virtual bool updateChannel(Channel* channel);
        virtual void removeChannel(Channel* channel);

        virtual bool hasChannel(Channel* channel) const;

        //static EPollPoller* newDefaultPoller(EventLoop* loop);

        void assertInLoopThread() const;

    private:
        static const int kInitEventListSize = 16;

        void fillActiveChannels(int numEvents, ChannelList* activeChannels, fd_set& readfds, fd_set& writefds) const;
        bool update(int operation, Channel* channel);

    private:
        typedef std::vector<struct epoll_event> EventList;

        int             m_epollfd;
        EventList       m_events;

        typedef std::map<int, Channel*> ChannelMap;

        ChannelMap      m_channels;
        EventLoop* m_ownerLoop;
    };

}
