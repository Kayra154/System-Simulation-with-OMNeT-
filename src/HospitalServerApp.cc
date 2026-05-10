#include "HospitalServerApp.h"
#include "LoRaApp/LoRaAppPacket_m.h"

using namespace flora;
Define_Module(HospitalServerApp);

HospitalServerApp::HospitalServerApp() {

}

HospitalServerApp::~HospitalServerApp() {

}

HospitalServerApp::HospitalServerApp(const HospitalServerApp &other) {

}

void HospitalServerApp::initialize(int stage)
{
    NetworkServerApp::initialize(stage);
    if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        expo_rate_lambda = par("expo_rate_lambda");
        queueSize = par("queueSize");
        urgentQueueCount = par("urgentQueue");
        normalQueueCount = par("normalQueue");
    }
}

void HospitalServerApp::scheduleNextService()
{
    if (urgentQueueCount > 0) {
        urgentQueueCount--;
        // schedule service completion after exponential time
        scheduleAt(simTime() + exponential(1.0/expo_rate_lambda),new cMessage("serviceComplete"));
    } else if (normalQueueCount > 0) {
        normalQueueCount--;
        scheduleAt(simTime() + exponential(1.0/expo_rate_lambda),new cMessage("serviceComplete"));
    }
}

void HospitalServerApp::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn("socketIn")) {
        auto pkt = check_and_cast<Packet *>(msg);
        const auto &frame = pkt->peekAtFront<LoRaMacFrame>();
        if (frame == nullptr)
            throw cRuntimeError("Header error type");

        if (simTime() >= getSimulation()->getWarmupPeriod()) {
            totalReceivedPackets++;

            try {
                auto appPkt = pkt->peekDataAt<LoRaAppPacket>(B(0), pkt->getDataLength());
                bool urgent = (appPkt->getSampleMeasurement() == 1);

                if (urgent) {
                    if(this->urgentQueueCount>=this->queueSize){
                        this->urgentDroppedCounter+=1;
                    }
                    else this->urgentQueueCount+=1;
                } else {
                    if(this->normalQueueCount>=this->queueSize){
                         this->normalDroppedCounter+=1;
                    }
                    else this->normalQueueCount+=1;
                }
            } catch (std::exception &e) {
                EV_WARN << "Could not parse LoRaAppPacket: " << e.what() << endl;
            }
        }

        updateKnownNodes(pkt);
        processLoraMACPacket(pkt);
    }
    else if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "serviceComplete") == 0) {
            delete msg;
            scheduleNextService();
        } else {
            processScheduledPacket(msg);
        }
    }
}
