/*
 * Copyright (C) 2011-2013 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2013 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SFSOAP_H
#define _SFSOAP_H

#include "Common.h"
#include "World.h"
#include "AccountMgr.h"
#include "Log.h"

#include "soapH.h"
#include "soapStub.h"
#include "stdsoap2.h"

#include <ace/Semaphore.h>
#include <ace/Task.h>

class SFSoapRunnable: public ACE_Based::Runnable
{
    public:
        SFSoapRunnable() { }
        void run();
        void setListenArguments(std::string host, uint16 port)
        {
            m_host = host;
            m_port = port;
        }
    private:
        void process_message(ACE_Message_Block* mb);

        std::string m_host;
        uint16 m_port;
};

class SOAPCommand
{
    public:
        SOAPCommand():
            pendingCommands(0, USYNC_THREAD, "pendingCommands"), m_success(false) {}

        ~SOAPCommand() {}

        void appendToPrintBuffer(const char* msg)
        {
            m_printBuffer += msg;
        }

        ACE_Semaphore pendingCommands;

        void setCommandSuccess(bool val)
        {
            m_success = val;
        }

        bool hasCommandSucceeded() const
        {
            return m_success;
        }

        static void print(void* callbackArg, const char* msg)
        {
            ((SOAPCommand*)callbackArg)->appendToPrintBuffer(msg);
        }

        static void commandFinished(void* callbackArg, bool success);

        bool m_success;
        std::string m_printBuffer;
};

#endif
