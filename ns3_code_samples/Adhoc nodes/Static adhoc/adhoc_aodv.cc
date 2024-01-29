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


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ADHOC_AODV_example");
//all adhoc nodes

int
main(int argc, char* argv[])
{

		
    bool verbose = true;
    uint32_t nWifi = 7;

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

 
    NodeContainer wifiNodes;
    wifiNodes.Create(nWifi);
    

    YansWifiPhyHelper wifiPhy;
    YansWifiChannelHelper channel ;			//YansWifiChannelHelper channel = YansWifiChannelHelper::Default();    
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(35.0));			//if made 0 then only tx no rcv.
    wifiPhy.SetChannel(channel.Create());


/*

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");

    WifiHelper wifi;

    //NetDeviceContainer staDevices;
    //mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    //staDevices = wifi.Install(phy, mac, wifiStaNodes);

    NetDeviceContainer wifiNodesDevices;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    wifiNodesDevices = wifi.Install(phy, mac, wifiNodes);

*/  
    
    
    //special wifiMac config for mobile nodes:
	WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac");
    
    WifiHelper wifi;
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode",
                                 StringValue("OfdmRate6Mbps"),
                                 "RtsCtsThreshold",
                                 UintegerValue(0));
                                 
    NetDeviceContainer wifiNodesDevices;
    wifiNodesDevices = wifi.Install(wifiPhy, wifiMac, wifiNodes);
    
    

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(10.0),
                                  "DeltaY",
                                  DoubleValue(30.0),
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
    mobility.Install(wifiNodes);				
    
    //last node:	
  	Ptr<ConstantPositionMobilityModel> s1 = wifiNodes.Get (nWifi-1)->GetObject<ConstantPositionMobilityModel> ();
  	s1->SetPosition (Vector ( 40.0, 40.0, 0  ));
	  

	//aodv:
    InternetStackHelper stack;
	AodvHelper aodv;
    stack.SetRoutingHelper(aodv); 
    stack.Install(wifiNodes);

	//routes:
    Ptr<OutputStreamWrapper> routingStream =
    Create<OutputStreamWrapper>("adhoc_aodv.routes", std::ios::out);
    Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(8), routingStream);


    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    
    Ipv4InterfaceContainer wifiNodesInterfaces;    
	wifiNodesInterfaces = address.Assign(wifiNodesDevices);
    
    
    //print - Client : 0 and server : last
	uint32_t nS = wifiNodes.Get(nWifi-1)->GetId();
	std::cout << "Node ID of server: " << nS << std::endl;
	Ipv4Address a2 = wifiNodesInterfaces.GetAddress(nWifi-1);
	std::cout << "IP address of server: " << a2 << std::endl << std::endl;	
	
	uint32_t nC = wifiNodes.Get(0)->GetId();
	std::cout << "Node ID of client: " << nC << std::endl;
    Ipv4Address a1 = wifiNodesInterfaces.GetAddress(0);
	std::cout << "IP address of client: " << a1 << std::endl << std::endl;
	
	
	
	//application, UDP:
    UdpEchoServerHelper echoServer(3456);

    ApplicationContainer serverApps = echoServer.Install(wifiNodes.Get(nWifi-1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(wifiNodesInterfaces.GetAddress(nWifi-1), 3456);
    echoClient.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps = echoClient.Install(wifiNodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    //Ipv4GlobalRoutingHelper::PopulateRoutingTables();


    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}







