//
// Created by danter on 2018-01-15.
//

#include "SoupBinEncoder.h"
#include "gtest/gtest.h"

TEST(SoupBinEncoderTest, test_encode_string1)
{
    std::string msg = "BDTe;i1071;SiNX;s1;TEt11;SYm2;NAmNASDAQ OMX Nordic;";

    char buf[1000] = { 0 };
    soupbin::encode(buf, msg);

    std::cout << "buf: " << buf << std::endl;

}
