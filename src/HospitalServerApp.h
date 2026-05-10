//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef HOSPITALSERVERAPP_H_
#define HOSPITALSERVERAPP_H_

#include "LoRa/NetworkServerApp.h"

class HospitalServerApp final : public flora::NetworkServerApp {
private:
    double expo_rate_lambda;
    uint8_t queueSize;
    uint8_t urgentQueueCount;
    uint8_t normalQueueCount;
    uint16_t urgentDroppedCounter;
    uint16_t normalDroppedCounter;

public:
    HospitalServerApp();
    virtual ~HospitalServerApp();
    HospitalServerApp(const HospitalServerApp &other);
    void initialize(int);
    virtual void handleMessage(cMessage *msg) override;
    void scheduleNextService();
};

#endif /* HOSPITALSERVERAPP_H_ */
