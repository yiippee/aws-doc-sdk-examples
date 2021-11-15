
/*
Purpose:
receive_message.cpp demonstrates how to receive and delete a message from an Amazon SQS queue.





// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX - License - Identifier: Apache - 2.0
//snippet-start:[sqs.cpp.receive_message.inc]
#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>
//snippet-end:[sqs.cpp.receive_message.inc]
//snippet-start:[sqs.cpp.delete_message.inc]
#include <aws/sqs/model/DeleteMessageRequest.h>
//snippet-end:[sqs.cpp.delete_message.inc]
#include <iostream>

void ReceiveMessage(const Aws::String& queue_url)
{
    // Let's make sure the request timeout is larger than the maximum possible
    // long poll time so that valid ReceiveMessage requests don't fail on long
    // poll queues
    Aws::Client::ClientConfiguration client_cfg;
    client_cfg.requestTimeoutMs = 30000;

    // snippet-start:[sqs.cpp.receive_message.code]
    Aws::SQS::SQSClient sqs(client_cfg);

    Aws::SQS::Model::ReceiveMessageRequest rm_req;
    rm_req.SetQueueUrl(queue_url);
    rm_req.SetMaxNumberOfMessages(1);

    auto rm_out = sqs.ReceiveMessage(rm_req);
    if (!rm_out.IsSuccess())
    {
        std::cout << "Error receiving message from queue " << queue_url << ": "
            << rm_out.GetError().GetMessage() << std::endl;
        return;
    }

    const auto& messages = rm_out.GetResult().GetMessages();
    if (messages.size() == 0)
    {
        std::cout << "No messages received from queue " << queue_url <<
            std::endl;
        return;
    }

    const auto& message = messages[0];
    std::cout << "Received message:" << std::endl;
    std::cout << "  MessageId: " << message.GetMessageId() << std::endl;
    std::cout << "  ReceiptHandle: " << message.GetReceiptHandle() << std::endl;
    std::cout << "  Body: " << message.GetBody() << std::endl << std::endl;
    // snippet-end:[sqs.cpp.receive_message.code]

    // snippet-start:[sqs.cpp.delete_message.code]
    Aws::SQS::Model::DeleteMessageRequest dm_req;
    dm_req.SetQueueUrl(queue_url);
    dm_req.SetReceiptHandle(message.GetReceiptHandle());

    auto dm_out = sqs.DeleteMessage(dm_req);
    if (dm_out.IsSuccess())
    {
        std::cout << "Successfully deleted message " << message.GetMessageId()
            << " from queue " << queue_url << std::endl;
    }
    else
    {
        std::cout << "Error deleting message " << message.GetMessageId() <<
            " from queue " << queue_url << ": " <<
            dm_out.GetError().GetMessage() << std::endl;
    }
    // snippet-end:[sqs.cpp.delete_message.code]
}

/**
 * Receives (and deletes) a message from an sqs queue based on command line
 * input
 */
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: receive_message <queue_url>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::String queue_url = argv[1];

        ReceiveMessage(queue_url);
    }
    Aws::ShutdownAPI(options);
    return 0;
}

