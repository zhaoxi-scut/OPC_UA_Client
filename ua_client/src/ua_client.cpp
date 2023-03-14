#include "ua_client.h"

using namespace std;
using namespace ua;

UaClient::UaClient(QObject *parent)
    : QObject{parent}
{
}

tuple<UA_UInt16, UA_Double, UA_Double, UA_Double, UA_Double> UaClient::readCameraVariable(const UA_NodeId &node_id)
{
    UA_NodeId exposure_id = __client.findNodeId(node_id, 1, "Exposure");
    UA_NodeId gain_id = __client.findNodeId(node_id, 1, "Gain");
    UA_NodeId r_gain_id = __client.findNodeId(node_id, 1, "RedGain");
    UA_NodeId g_gain_id = __client.findNodeId(node_id, 1, "GreenGain");
    UA_NodeId b_gain_id = __client.findNodeId(node_id, 1, "BlueGain");

    UA_UInt16 exposure = *reinterpret_cast<UA_UInt16 *>(__client.readVariable(exposure_id).get().data);
    UA_Double gain = *reinterpret_cast<UA_Double *>(__client.readVariable(gain_id).get().data);
    UA_Double r_gain = *reinterpret_cast<UA_Double *>(__client.readVariable(r_gain_id).get().data);
    UA_Double g_gain = *reinterpret_cast<UA_Double *>(__client.readVariable(g_gain_id).get().data);
    UA_Double b_gain = *reinterpret_cast<UA_Double *>(__client.readVariable(b_gain_id).get().data);
    return {exposure, gain, r_gain, g_gain, b_gain};
}

tuple<array<UA_Byte, 4>, array<UA_UInt16, 4>> UaClient::readLightControllerVariable(const UA_NodeId &node_id)
{
    UA_NodeId lum_id = __client.findNodeId(node_id, 1, "Luminance");
    UA_NodeId del_id = __client.findNodeId(node_id, 1, "Delay");

    UA_Variant lum_val = __client.readVariable(lum_id).get();
    UA_Variant del_val = __client.readVariable(del_id).get();

    UA_Byte *lum_data = reinterpret_cast<UA_Byte *>(lum_val.data);
    UA_UInt16 *del_data = reinterpret_cast<UA_UInt16 *>(del_val.data);
    size_t lum_size = lum_val.arrayLength;
    size_t del_size = del_val.arrayLength;
    Q_ASSERT(lum_size == 4 && del_size == 4);
    array<UA_Byte, 4> luminance;
    for (size_t i = 0; i < luminance.size(); ++i)
        luminance[i] = lum_data[i];
    array<UA_UInt16, 4> delay;
    for (size_t i = 0; i < delay.size(); ++i)
        delay[i] = del_data[i];
    return {luminance, delay};
}
