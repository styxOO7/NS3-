// /*
//  * This program is free software; you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License version 2 as
//  * published by the Free Software Foundation;
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program; if not, write to the Free Software
//  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  */

// #include "ns3/applications-module.h"
// #include "ns3/core-module.h"
// #include "ns3/csma-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/mobility-module.h"
// #include "ns3/network-module.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/ssid.h"
// #include "ns3/yans-wifi-helper.h"
// #include "ns3/netanim-module.h"

// // Default Network Topology
// //
// //   Wifi 10.1.3.0
// //                 AP
// //  *    *    *    *
// //  |    |    |    |    10.1.1.0
// // n5   n6   n7   n0 -------------- n1   n2   n3   n4
// //                   point-to-point  |    |    |    |
// //                                   ================
// //                                     LAN 10.1.2.0

// using namespace ns3;

// NS_LOG_COMPONENT_DEFINE("ThirdScriptExample");

// int
// main(int argc, char* argv[])
// {
//     bool verbose = true;
//     uint32_t nCsma = 3;
//     uint32_t nWifi = 3;
//     bool tracing = false;

//     CommandLine cmd(__FILE__);
//     cmd.AddValue("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
//     cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
//     cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
//     cmd.AddValue("tracing", "Enable pcap tracing", tracing);

//     cmd.Parse(argc, argv);

//     // The underlying restriction of 18 is due to the grid position
//     // allocator's configuration; the grid layout will exceed the
//     // bounding box if more than 18 nodes are provided.
//     if (nWifi > 18)
//     {
//         std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box"
//                   << std::endl;
//         return 1;
//     }

//     if (verbose)
//     {
//         LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
//         LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
//     }

// //p2p network:
//     NodeContainer p2pNodes;
//     p2pNodes.Create(2);

//     PointToPointHelper pointToPoint;		//object
//     pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
//     pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

//     NetDeviceContainer p2pDevices;
//     p2pDevices = pointToPoint.Install(p2pNodes);

// //csma network:
//     NodeContainer csmaNodes;
//     csmaNodes.Add(p2pNodes.Get(1));
//     csmaNodes.Create(nCsma);

//     CsmaHelper csma;			//object
//     csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
//     csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

//     NetDeviceContainer csmaDevices;
//     csmaDevices = csma.Install(csmaNodes);

// //wifi network:
//     NodeContainer wifiStaNodes;
//     wifiStaNodes.Create(nWifi);		//n5,n6,n7
//     NodeContainer wifiApNode = p2pNodes.Get(0);		//n0 access point of wifi, central hub, other device connect at this point and get internet

// /*
// in wifi, channels concept is used which is to introduce real world effects like attenutaion constant and prop. delay etc.
// for wired connections, channels arent required as properties can be setted on nodes itself.
// first channel then wifi physical layered object then integration.
// */


// 	//physical layer:-----------------------------
//     YansWifiChannelHelper channel = YansWifiChannelHelper::Default();	//channel
//     YansWifiPhyHelper phy;		//object
//     phy.SetChannel(channel.Create());		//set channel of wifiPhy object



// 	//main wifi object:
//     WifiHelper wifi;
//     //wifi.SetRemoteStationManager("ns3::AarfWifiManager");
    
    
    
// 	//data link layer:----------------------------
// 	//servie set ID - human readable identifier different in terms of encryption.
	
//     WifiMacHelper mac;
//     Ssid ssid = Ssid("ns-3-ssid");

// 	//define mac layer different for AP and Station:
// 	// AP - gives instruction.
// 	// Station - follows instruction from AP.
	
//     mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));	
//     NetDeviceContainer staDevices;
//     staDevices = wifi.Install(phy, mac, wifiStaNodes);



//     mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
//     NetDeviceContainer apDevices;
//     apDevices = wifi.Install(phy, mac, wifiApNode);



//     MobilityHelper mobility;

//     mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                                   "MinX",
//                                   DoubleValue(0.0),
//                                   "MinY",
//                                   DoubleValue(0.0),
//                                   "DeltaX",
//                                   DoubleValue(5.0),
//                                   "DeltaY",
//                                   DoubleValue(10.0),
//                                   "GridWidth",
//                                   UintegerValue(3),
//                                   "LayoutType",
//                                   StringValue("RowFirst"));

//     mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
//                               "Bounds",
//                               RectangleValue(Rectangle(-50, 50, -50, 50)));
//     mobility.Install(wifiStaNodes);

// 	//fixed AP node:
//     mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//     mobility.Install(wifiApNode);

//     InternetStackHelper stack;
//     stack.Install(csmaNodes);
//     stack.Install(wifiApNode);
//     stack.Install(wifiStaNodes);

//     Ipv4AddressHelper address;

//     address.SetBase("172.11.1.0", "255.255.255.0");
//     Ipv4InterfaceContainer p2pInterfaces;
//     p2pInterfaces = address.Assign(p2pDevices);

//     address.SetBase("172.11.2.0", "255.255.255.0");
//     Ipv4InterfaceContainer csmaInterfaces;
//     csmaInterfaces = address.Assign(csmaDevices);

//     address.SetBase("172.11.3.0", "255.255.255.0");
//     address.Assign(staDevices);
//     address.Assign(apDevices);

//     UdpEchoServerHelper echoServer(100);

//     ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));	//n4
//     serverApps.Start(Seconds(1.0));
//     serverApps.Stop(Seconds(10.0));

//     UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma), 100);
//     echoClient.SetAttribute("MaxPackets", UintegerValue(2));
//     echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
//     echoClient.SetAttribute("PacketSize", UintegerValue(2048));

//     ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(nWifi - 1));	//n7
//     clientApps.Start(Seconds(2.0));
//     clientApps.Stop(Seconds(10.0));

//     Ipv4GlobalRoutingHelper::PopulateRoutingTables();

//     Simulator::Stop(Seconds(10.0));

//     if (tracing)
//     {
//         phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
//         pointToPoint.EnablePcapAll("p2p");
//         phy.EnablePcap("wireless", apDevices.Get(0));
//         csma.EnablePcap("csma", csmaDevices.Get(0), true);
//     }
    
//     //animation:
    
//     /*
//     p2p nodes - n0 n1 : 0 1
//     csma nodes - n2 n3 n4 : 1 2 3 (n0 added)
//     wireless station nodes - n5 n6 n7 : 0 1 2
    
//     */

//     AnimationInterface anim("myThird.xml");
// 	AnimationInterface::SetConstantPosition(p2pNodes.Get(0),40,20);
// 	AnimationInterface::SetConstantPosition(p2pNodes.Get(1),60,20);
// 	AnimationInterface::SetConstantPosition(csmaNodes.Get(1),70,20);
// 	AnimationInterface::SetConstantPosition(csmaNodes.Get(2),80,20);
// 	AnimationInterface::SetConstantPosition(csmaNodes.Get(3),90,20);
// 	// AnimationInterface::SetConstantPosition(wifiStaNodes.Get(0),10,20);
// 	// AnimationInterface::SetConstantPosition(wifiStaNodes.Get(1),20,20);
// 	// AnimationInterface::SetConstantPosition(wifiStaNodes.Get(2),30,20);
// 	anim.EnablePacketMetadata(true);

	
	
// 	/*
// 	//ascii:
// 	AsciiTraceHelper ascii;
// 	phy.EnableAsciiAll(ascii.CreateFileStream("phy.tr"));
// 	csma.EnableAsciiAll(ascii.CreateFileStream("csma.tr"));
// 	pointToPoint.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));
//     */

//     Simulator::Run();
//     Simulator::Destroy();
//     return 0;
// }





//TESTING:
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                                     LAN 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ThirdScriptExample");

int
main(int argc, char* argv[])
{
    bool verbose = true;
    uint32_t nCsma = 3;
    uint32_t nWifi = 3;
    bool tracing = true;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
    cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);

    cmd.Parse(argc, argv);

    // The underlying restriction of 18 is due to the grid position
    // allocator's configuration; the grid layout will exceed the
    // bounding box if more than 18 nodes are provided.
    if (nWifi > 18)
    {
        std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box"
                  << std::endl;
        return 1;
    }

    if (verbose)
    {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    //p2p network:
    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    PointToPointHelper pointToPoint;		//object
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    //csma network:
    NodeContainer csmaNodes;
    csmaNodes.Add(p2pNodes.Get(1));
    csmaNodes.Create(3);

    CsmaHelper csma;			//object
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    //wifi network:
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(3);		//n5,n6,n7
    NodeContainer wifiApNode = p2pNodes.Get(0);		//n0 access point of wifi, central hub, other device connect at this point and get internet

	//physical layer:-----------------------------
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();	//channel
    YansWifiPhyHelper phy;		//object
    phy.SetChannel(channel.Create());		//set channel of wifiPhy object

	//main wifi object:
    WifiHelper wifi;

	//data link layer:----------------------------
    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");
	
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));	
    NetDeviceContainer staDevices;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);


    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    NetDeviceContainer apDevices;
    apDevices = wifi.Install(phy, mac, wifiApNode);



    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(5.0),
                                  "DeltaY",
                                  DoubleValue(10.0),
                                  "GridWidth",
                                  UintegerValue(3),
                                  "LayoutType",
                                  StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds",
                              RectangleValue(Rectangle(-50, 50, -50, 50)));
    mobility.Install(wifiStaNodes);

	//fixed AP node:
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNode);


    InternetStackHelper stack;
    stack.Install(csmaNodes);
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);

    address.SetBase("10.1.3.0", "255.255.255.0");
    address.Assign(staDevices);
    address.Assign(apDevices);


    //server:
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));	//n4
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    //for csma:
    UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma), 9); //send req to n4
    echoClient.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(nWifi-1)); //n7
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
 	pointToPoint.EnablePcapAll ("Thirdp2p7");
   	csma.EnablePcapAll("ThirdCsma");
	phy.EnablePcapAll("ThirdWifi");
  


    if (tracing)
    {
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        pointToPoint.EnablePcapAll("p2p_3");
        phy.EnablePcap("wireless_3", apDevices.Get(0));
        csma.EnablePcap("csma_3", csmaDevices.Get(0), true);
    }
    
    
    /*
    p2p nodes - n0 n1 : 0 1
    csma nodes - n2 n3 n4 : 1 2 3 (n0 added)
    wireless station nodes - n5 n6 n7 : 0 1 2    
    */

    //animation:
    AnimationInterface anim("WIRELESS.xml");
    anim.SetConstantPosition(p2pNodes.Get(0),10.0,10.0);
    anim.SetConstantPosition(csmaNodes.Get(0),20.0,20.0);
    anim.SetConstantPosition(csmaNodes.Get(1),30.0,30.0);
    anim.SetConstantPosition(csmaNodes.Get(2),40.0,40.0);
    anim.SetConstantPosition(csmaNodes.Get(3),50.0,50.0);
	// AnimationInterface::SetConstantPosition(p2pNodes.Get(0),40,20);
	// AnimationInterface::SetConstantPosition(p2pNodes.Get(1),60,20);
	// AnimationInterface::SetConstantPosition(csmaNodes.Get(1),70,20);
	// AnimationInterface::SetConstantPosition(csmaNodes.Get(2),80,20);
	// AnimationInterface::SetConstantPosition(csmaNodes.Get(3),90,20);
	anim.EnablePacketMetadata(true);

	
    Simulator::Stop(Seconds(10.0));
	
	
	//ascii:
	AsciiTraceHelper ascii;
	phy.EnableAsciiAll(ascii.CreateFileStream("phy.tr"));
	csma.EnableAsciiAll(ascii.CreateFileStream("csma.tr"));
	pointToPoint.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));
    

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
