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

#ifndef PATIENTSENSORAPP_H_
#define PATIENTSENSORAPP_H_

#include "LoRaApp/SimpleLoRaApp.h"
#include "LoRa/LoRaTagInfo_m.h"
#include "LoRaApp/LoRaAppPacket_m.h"

using namespace flora;

class PatientSensorApp final : public flora::SimpleLoRaApp{
private:
    double uniform_rate_IAT= par("uniform_rate_IAT");
    double expo_rate_lambda= par("expo_rate_lambda");
public:
    void initialize(int);
    PatientSensorApp();
    virtual ~PatientSensorApp();
    PatientSensorApp(const PatientSensorApp &other);
    virtual void handleMessage(cMessage*);
    virtual void handleMessageFromLowerLayer(cMessage *msg);
    virtual void sendJoinRequest();
};

#endif /* PATIENTSENSORAPP_H_ */
