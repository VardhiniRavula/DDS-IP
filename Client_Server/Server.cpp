#include "SpeedDataPubSubTypes.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace eprosima::fastdds::dds;

int main()
{s
    DomainParticipant* participant = DomainParticipantFactory::get_instance()
        ->create_participant_with_profile(0, "VehicleParticipant");

    TypeSupport type(new SpeedDataPubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("SpeedTopic", "SpeedData", TOPIC_QOS_DEFAULT);
    Publisher* publisher = participant->create_publisher_with_profile("Server");
    DataWriter* writer = publisher->create_datawriter_with_profile(topic, "SpeedWriter");

    SpeedData data;
    int speed_value = 0;

    while (true)
    {
        data.speed = static_cast<float>(speed_value);
        writer->write(&data);
        std::cout << "Published speed: " << data.speed << " km/h" << std::endl;
        speed_value = (speed_value + 10) % 200;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
