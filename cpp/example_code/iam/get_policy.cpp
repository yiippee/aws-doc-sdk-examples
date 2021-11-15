// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX - License - Identifier: Apache - 2.0
/*
Purpose:
get_policy.cpp demonstrates how to retrieve information about an IAM managed policy.]
*/



//snippet-start:[iam.cpp.get_policy.inc]
#include <aws/core/Aws.h>
#include <aws/iam/IAMClient.h>
#include <aws/iam/model/GetPolicyRequest.h>
#include <aws/iam/model/GetPolicyResult.h>
#include <iostream>
//snippet-end:[iam.cpp.get_policy.inc]

/**
 * Gets an IAM policy's information, based on command line input
 */
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: get_policy <policy_arn>" << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::String policy_arn(argv[1]);

        // snippet-start:[iam.cpp.get_policy.code]
        Aws::IAM::IAMClient iam;
        Aws::IAM::Model::GetPolicyRequest request;
        request.SetPolicyArn(policy_arn);

        auto outcome = iam.GetPolicy(request);
        if (!outcome.IsSuccess())
        {
            std::cout << "Error getting policy " << policy_arn << ": " <<
                outcome.GetError().GetMessage() << std::endl;
        }
        else
        {
            const auto &policy = outcome.GetResult().GetPolicy();
            std::cout << "Name: " << policy.GetPolicyName() << std::endl <<
                "ID: " << policy.GetPolicyId() << std::endl << "Arn: " <<
                policy.GetArn() << std::endl << "Description: " <<
                policy.GetDescription() << std::endl << "CreateDate: " <<
                policy.GetCreateDate().ToGmtString(Aws::Utils::DateFormat::ISO_8601)
                << std::endl;
        }
        // snippet-end:[iam.cpp.get_policy.code]
    }
    Aws::ShutdownAPI(options);
    return 0;
}

