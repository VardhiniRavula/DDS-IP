#include "SpeedDataPubSubTypes.h"
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <iostream>

using namespace eprosima::fastdds::dds;

class SpeedListener : public DataReaderListener
{
public:
    void on_data_available(DataReader* reader) override
    {
        SpeedData data;
        SampleInfo info;
        if (reader->take_next_sample(&data, &info) == ReturnCode_t::RETCODE_OK && info.valid_data)
        {
            std::cout << "Received speed: " << data.speed << " km/h" << std::endl;
        }
    }
};

int main()
{
    DomainParticipant* participant = DomainParticipantFactory::get_instance()
        ->create_participant_with_profile(0, "VehicleParticipant");

    TypeSupport type(new SpeedDataPubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("SpeedTopic", "SpeedData", TOPIC_QOS_DEFAULT);
    Subscriber* subscriber = participant->create_subscriber_with_profile("Client");

    SpeedListener listener;
    DataReader* reader = subscriber->create_datareader_with_profile(topic, "SpeedReader", &listener);

    std::cout << "Waiting for speed data..." << std::endl;
    while (true) std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
