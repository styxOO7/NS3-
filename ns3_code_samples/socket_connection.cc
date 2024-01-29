
// ------------------------------------------------------------------------TESTING STARTS---------------------------------------------------------------------------


#include "iostream"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UdpExample");

// Custom function to print log statements for data transmission
void PrintDataTransmissionLog (Ptr<const Packet> packet, const Address& address)
{
  NS_LOG_INFO ("Node 0 sent a packet to Node 1 at " << Simulator::Now ().GetSeconds () << " seconds.");
}

// Custom function to print log statements for data reception
void PrintDataReceptionLog (Ptr<const Packet> packet, const Address& address)
{
  NS_LOG_INFO ("Node 1 received a packet from Node 0 at " << Simulator::Now ().GetSeconds () << " seconds.");
}

int main (int argc, char *argv[])
{
  LogComponentEnable ("UdpExample", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpExample", LOG_LEVEL_DEBUG);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  Ptr<Socket> sourceSocket = Socket::CreateSocket (nodes.Get (0), TypeId::LookupByName ("ns3::UdpSocketFactory"));
  sourceSocket->Bind (InetSocketAddress (interfaces.GetAddress (0), 9));

  Ptr<Socket> sinkSocket = Socket::CreateSocket (nodes.Get (1), TypeId::LookupByName ("ns3::UdpSocketFactory"));
  sinkSocket->Bind (InetSocketAddress (interfaces.GetAddress (1), 9));

  // Set up the packet sink application at Node1:
  PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (10.0));

  // Set up the OnOff application to send UDP traffic at Node0:
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", Address (InetSocketAddress (interfaces.GetAddress (1), 9)));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1024));
  onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));

  // Set up custom callbacks for logging
  std:: cout << "Here entered" << std::endl;
  //NS_LOG_INFO ("Node 0 sent a packet to Node 1 at ");
  //sourceSocket->TraceConnectWithoutContext ("Send", MakeCallback (&PrintDataTransmissionLog));
  //sinkSocket->TraceConnectWithoutContext ("Rx", MakeCallback (&PrintDataReceptionLog));

  pointToPoint.EnablePcapAll("socket_connection_packets");
	

  ApplicationContainer apps = onOffHelper.Install (nodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (5.0));

  AnimationInterface anim("socket_connection.xml");
  anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
  anim.SetConstantPosition(nodes.Get(1), 20.0, 20.0);

  // Run simulation
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}




















// ------------------------------------------------------------------------TESTING ENDS---------------------------------------------------------------------------



























// ------------------------------------------------------------------------VERIFIED FILE---------------------------------------------------------------------------


/*


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UdpExample");




//source node0
//rcvr node1




int main (int argc, char *argv[])
{
  // Log components
  LogComponentEnable ("UdpExample", LOG_LEVEL_INFO);
LogComponentEnable ("UdpExample", LOG_LEVEL_DEBUG);


  // Create nodes
  NodeContainer nodes;
  nodes.Create (2);

  // Create point-to-point link
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  // Install internet stack
  InternetStackHelper stack;
  stack.Install (nodes);

  // Assign IPv4 addresses
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // Create UDP socket at node 0
  Ptr<Socket> sourceSocket = Socket::CreateSocket (nodes.Get (0), TypeId::LookupByName ("ns3::UdpSocketFactory"));

  // Set socket to be bound to the interface (IP)
  sourceSocket->Bind (InetSocketAddress (interfaces.GetAddress (0), 9));

  // Create UDP socket at node 1
  Ptr<Socket> sinkSocket = Socket::CreateSocket (nodes.Get (1), TypeId::LookupByName ("ns3::UdpSocketFactory"));

  // Set socket to be bound to the interface (IP)
  sinkSocket->Bind (InetSocketAddress (interfaces.GetAddress (1), 9));

  // Set up the packet sink application at Node1:
  PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
  ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (Seconds (10.0));

  // Set up the OnOff application to send UDP traffic at Node0 -> continuous in nature:
  // app started at 1 second and initial state is OFF
  // 1 to 2 OFF
  // 2 to 3 ON -> in netanim
  // 3 to 4 OFF
  // 4 to 5 ON -> in netanim
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", Address (InetSocketAddress (interfaces.GetAddress (1), 9)));	//add to send traffic at
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1024));
  onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));

  ApplicationContainer apps = onOffHelper.Install (nodes.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (5.0));
  
  //animation:
	AnimationInterface anim("socket_connection.xml");
	anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
	anim.SetConstantPosition(nodes.Get(1), 20.0, 20.0);




  // Run simulation
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

*/
