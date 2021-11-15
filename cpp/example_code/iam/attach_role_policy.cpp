// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX - License - Identifier: Apache - 2.0
/*
Purpose:
attach_role_policy.cpp demonstrates how to attach a managed policy to an IAM role.]
*/

//snippet-start:[iam.cpp.attach_role_policy.inc]
#include <aws/core/Aws.h>
#include <aws/iam/IAMClient.h>
#include <aws/iam/model/AttachRolePolicyRequest.h>
#include <aws/iam/model/ListAttachedRolePoliciesRequest.h>
#include <aws/iam/model/ListAttachedRolePoliciesResult.h>
#include <iostream>
#include <iomanip>
//snippet-end:[iam.cpp.attach_role_policy.inc]

void AttachRolePolicy(
    const Aws::String& role_name, const Aws::String& policy_arn)
{
    // snippet-start:[iam.cpp.attach_role_policy.code]
    Aws::IAM::IAMClient iam;

    Aws::IAM::Model::ListAttachedRolePoliciesRequest list_request;
    list_request.SetRoleName(role_name);

    bool done = false;
    while (!done)
    {
        auto list_outcome = iam.ListAttachedRolePolicies(list_request);
        if (!list_outcome.IsSuccess())
        {
            std::cout << "Failed to list attached policies of role " <<
                role_name << ": " << list_outcome.GetError().GetMessage() <<
                std::endl;
            return;
        }

        const auto& policies = list_outcome.GetResult().GetAttachedPolicies();
        if (std::any_of(policies.cbegin(), policies.cend(),
            [=](const Aws::IAM::Model::AttachedPolicy& policy)
        {
            return policy.GetPolicyArn() == policy_arn;
        }))
        {
            std::cout << "Policy " << policy_arn <<
                " is already attached to role " << role_name << std::endl;
            return;
        }

        done = !list_outcome.GetResult().GetIsTruncated();
        list_request.SetMarker(list_outcome.GetResult().GetMarker());
    }

    Aws::IAM::Model::AttachRolePolicyRequest request;
    request.SetRoleName(role_name);
    request.SetPolicyArn(policy_arn);

    auto outcome = iam.AttachRolePolicy(request);
    if (!outcome.IsSuccess())
    {
        std::cout << "Failed to attach policy " << policy_arn << " to role " <<
            role_name << ": " << outcome.GetError().GetMessage() << std::endl;
        return;
    }

    std::cout << "Successfully attached policy " << policy_arn << " to role " <<
        role_name << std::endl;
    // snippet-end:[iam.cpp.attach_role_policy.code]
}

static const char* SAMPLE_POLICY_ARN =
"arn:aws:iam::aws:policy/AmazonDynamoDBFullAccess";

/**
 * Attaches a policy to a role, based on command line input
 */
int main(int argc, char** argv)
{
    if (argc < 2 || argc >= 4)
    {
        std::cout << "Usage: attach_role_policy <role_name> [policy_arn]" <<
            std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::String role_name(argv[1]);
        Aws::String policy_arn;
        if (argc == 3)
        {
            policy_arn = argv[2];
        }
        else
        {
            policy_arn = SAMPLE_POLICY_ARN;
        }

        AttachRolePolicy(role_name, policy_arn);
    }
    Aws::ShutdownAPI(options);
    return 0;
}
