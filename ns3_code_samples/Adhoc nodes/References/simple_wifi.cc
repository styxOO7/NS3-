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

#include "ns3/aodv-module.h"
#include "iostream"
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
//  |    |    |    |    
// n0   n1   n2   n3 

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ThirdScriptExample");

int
main(int argc, char* argv[])
{

		
    bool verbose = true;
    uint32_t nWifi = 6;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
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

 
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(nWifi);
    
    NodeContainer wifiApNode;
    wifiApNode.Create(1);

    YansWifiChannelHelper channel ;			//YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    
    YansWifiPhyHelper phy;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(35.0));			//if made 0 then only tx no rcv.
	
    phy.SetChannel(channel.Create());



    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");

    WifiHelper wifi;

    NetDeviceContainer staDevices;
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    staDevices = wifi.Install(phy, mac, wifiStaNodes);

    NetDeviceContainer apDevices;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    apDevices = wifi.Install(phy, mac, wifiApNode);

    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(10.0),
                                  "DeltaY",
                                  DoubleValue(20.0),
                                  "GridWidth",
                                  UintegerValue(3),
                                  "LayoutType",
                                  StringValue("RowFirst"));

	/*
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds",
                              RectangleValue(Rectangle(-10, 10, -10, 10)));
    */
    
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");	
    mobility.Install(wifiStaNodes);				//mobile Station nodes
    
    
    //AP node position:
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");		//fixed AP
    mobility.Install(wifiApNode);
	
	  Ptr<ConstantPositionMobilityModel> s1 = wifiApNode.Get (0)->GetObject<ConstantPositionMobilityModel> ();
	  s1->SetPosition (Vector ( 30.0, 30.0, 0  ));
	  
	  /*
	  Ptr<ConstantPositionMobilityModel> s2 = wifiApNode.Get (1)->GetObject<ConstantPositionMobilityModel> ();
	  s2->SetPosition (Vector ( 25.0, 25.0, 0  ));
	*/

//aodv:

    InternetStackHelper stack;

	AodvHelper aodv;
    stack.SetRoutingHelper(aodv); // has effect on the next Install ()

    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

	//routes:
    Ptr<OutputStreamWrapper> routingStream =
    Create<OutputStreamWrapper>("simple_wifi.routes", std::ios::out);
    Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(8), routingStream);




    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    
    Ipv4InterfaceContainer wifiStaInterfaces;
    Ipv4InterfaceContainer wifiApInterfaces;
    
	wifiStaInterfaces = address.Assign(staDevices);
    wifiApInterfaces = address.Assign(apDevices);    
    
    
    //print
	uint32_t nS = wifiApNode.Get(0)->GetId();
	std::cout << "Node ID of server: " << nS << std::endl;
	Ipv4Address a2 = wifiApInterfaces.GetAddress(0);
	std::cout << "IP address of server: " << a2 << std::endl << std::endl;	
	
	/*
	uint32_t ap2 = wifiApNode.Get(1)->GetId();
	std::cout << "Node ID of mid apnode: " << ap2 << std::endl;
	Ipv4Address ap2_add = wifiApInterfaces.GetAddress(1);
	std::cout << "IP address of mid apnode: " << ap2_add << std::endl << std::endl;	
	*/
	
	uint32_t nC = wifiStaNodes.Get(0)->GetId();
	std::cout << "Node ID of client: " << nC << std::endl;
    Ipv4Address a1 = wifiStaInterfaces.GetAddress(0);
	std::cout << "IP address of client: " << a1 << std::endl << std::endl;
	
	
	

    UdpEchoServerHelper echoServer(3456);

    ApplicationContainer serverApps = echoServer.Install(wifiApNode.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(wifiApInterfaces.GetAddress(0), 3456);
    echoClient.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps = echoClient.Install(wifiStaNodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    //Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    
// ANIMATION PART START ---------------------------------------------------------------------------------------------------


    AnimationInterface anim("simple_wifi.xml");
    //anim.SetConstantPosition(p2pNodes.Get(0),10.0,10.0);
	anim.EnablePacketMetadata(true);



//ANIMATION PART END --------------------------------------------------------------------------------------------------


	//ascii:
	/*
	AsciiTraceHelper ascii;
	phy.EnableAsciiAll(ascii.CreateFileStream("simple_wifi_phy.tr"));
	*/

    Simulator::Stop(Seconds(10.0));

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}







