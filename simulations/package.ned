package test2;
import inet.applications.udpapp.UdpBasicApp;
import inet.node.inet.Router;
import inet.node.internetcloud.InternetCloud;
import flora.LoRaPhy.LoRaMedium;
import flora.LoraNode.LoRaNode;
import flora.LoraNode.LoRaGW;
import inet.node.inet.StandardHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth1G;
import inet.queueing.sink.ActivePacketSink;
import inet.queueing.queue.PacketQueue;
import inet.queueing.classifier.PriorityClassifier;
import inet.queueing.source.ActivePacketSource;

//simple Queue
//{
//    parameters:
//        double emergencyProbability = default(0.3);
//        volatile double meanServiceTime @unit(s) = default(0.05s);
//        @display("i=block/queue");
//    gates:
//        inout ethgIn @labels(EtherFrame);
//        inout ethgOut @labels(EtherFrame);
//}
@license(LGPL);
network LoRaNetworkTest
{
    submodules:
        loRaNode: LoRaNode {
            @display("p=320.565,410.865");
        }
        loRaGateway: LoRaGW {
            @display("p=276,325;is=s");
        }
        LoRaMedium: LoRaMedium {
            @display("p=833,64");
        }
        HospitalServer: StandardHost {
            parameters:
                @display("p=157,182");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                assignDisjunctSubnetAddresses = false;
                @display("p=830,167");
        }
        gwRouter: Router {
            @display("p=135,287");
        }
//        myQueue: Queue {
//            @display("p=248,238");   // Position between router and server
//        }
    connections:
        gwRouter.ethg++ <--> Eth1G <--> loRaGateway.ethg++;
//        gwRouter.ethg++ <--> Eth1G <--> myQueue.ethgIn;
//        myQueue.ethgOut <--> Eth1G <--> HospitalServer.ethg++;
		gwRouter.ethg++ <--> Eth1G <--> HospitalServer.ethg++;
}
