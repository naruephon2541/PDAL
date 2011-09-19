/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>

#include <pdal/external/boost/uuid/uuid_io.hpp>

#include <pdal/PointBuffer.hpp>
#include <pdal/SchemaLayout.hpp>
#include <pdal/drivers/faux/Reader.hpp>
#include <pdal/drivers/faux/Iterator.hpp>

#include <iostream>

#ifdef PDAL_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

using namespace pdal;

BOOST_AUTO_TEST_SUITE(FauxReaderTest)

BOOST_AUTO_TEST_CASE(test_constant_mode_sequential_iter)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 102.0, 103.0);
    pdal::drivers::faux::Reader reader(bounds, 1000, pdal::drivers::faux::Reader::Constant);
    reader.initialize();

    BOOST_CHECK_EQUAL(reader.getDescription(), "Faux Reader");

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 750);
 
    StageSequentialIterator* iter = reader.createSequentialIterator();
    boost::uint32_t numRead = iter->read(data);

    BOOST_CHECK_EQUAL(numRead, 750u);

    int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    for (boost::uint32_t i=0; i<numRead; i++)
    {
        double x = data.getField<double>(i, offsetX);
        double y = data.getField<double>(i, offsetY);
        double z = data.getField<double>(i, offsetZ);
        boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

        BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
        BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
        BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
        BOOST_CHECK_EQUAL(t, i);
    }

    delete iter;

    return;
}


BOOST_AUTO_TEST_CASE(FauxReaderTest_test_options)
{
    const Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 102.0, 103.0);
    Option opt1("bounds", bounds);
    Option opt2("mode", "conSTanT");
    Option opt3("num_points", 1000);
    Option opt4("id", 90210);
    Options opts;
    opts.add(opt1);
    opts.add(opt2);
    opts.add(opt3);
    opts.add(opt4);
    pdal::drivers::faux::Reader reader(opts);
    reader.initialize();

    BOOST_CHECK_EQUAL(reader.getDescription(), "Faux Reader");
    BOOST_CHECK_EQUAL(reader.getId(), 90210u);

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 750);
 
    StageSequentialIterator* iter = reader.createSequentialIterator();
    boost::uint32_t numRead = iter->read(data);

    BOOST_CHECK_EQUAL(numRead, 750u);

    int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    for (boost::uint32_t i=0; i<numRead; i++)
    {
        double x = data.getField<double>(i, offsetX);
        double y = data.getField<double>(i, offsetY);
        double z = data.getField<double>(i, offsetZ);
        boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

        BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
        BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
        BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
        BOOST_CHECK_EQUAL(t, i);
    }

    delete iter;

    return;
}


BOOST_AUTO_TEST_CASE(test_constant_mode_random_iter)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 102.0, 103.0);
    pdal::drivers::faux::Reader reader(bounds, 1000, pdal::drivers::faux::Reader::Constant);
    reader.initialize();

    BOOST_CHECK_EQUAL(reader.getDescription(), "Faux Reader");

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 10);

    int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    StageRandomIterator* iter = reader.createRandomIterator();

    boost::uint32_t numRead = iter->read(data);
    BOOST_CHECK_EQUAL(numRead, 10u);

    {
        for (boost::uint32_t i=0; i<numRead; i++)
        {
            double x = data.getField<double>(i, offsetX);
            double y = data.getField<double>(i, offsetY);
            double z = data.getField<double>(i, offsetZ);
            boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

            BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
            BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
            BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
            BOOST_CHECK_EQUAL(t, i);
        }
    }

    numRead = iter->read(data);
    BOOST_CHECK_EQUAL(numRead, 10u);

    {
        for (boost::uint32_t i=0; i<numRead; i++)
        {
            double x = data.getField<double>(i, offsetX);
            double y = data.getField<double>(i, offsetY);
            double z = data.getField<double>(i, offsetZ);
            boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

            BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
            BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
            BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
            BOOST_CHECK_EQUAL(t, i+10);
        }
    }

    boost::uint64_t newPos = iter->seek(99);
    BOOST_CHECK_EQUAL(newPos, 99);
    numRead = iter->read(data);
    BOOST_CHECK_EQUAL(numRead, 10u);

    {
        for (boost::uint32_t i=0; i<numRead; i++)
        {
            double x = data.getField<double>(i, offsetX);
            double y = data.getField<double>(i, offsetY);
            double z = data.getField<double>(i, offsetZ);
            boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

            BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
            BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
            BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
            BOOST_CHECK_EQUAL(t, i+99);

        }
    }

    newPos = iter->seek(7);
    BOOST_CHECK_EQUAL(newPos, 7);
    numRead = iter->read(data);
    BOOST_CHECK_EQUAL(numRead, 10u);

    {
        for (boost::uint32_t i=0; i<numRead; i++)
        {
            double x = data.getField<double>(i, offsetX);
            double y = data.getField<double>(i, offsetY);
            double z = data.getField<double>(i, offsetZ);
            boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

            BOOST_CHECK_CLOSE(x, 1.0, 0.00001);
            BOOST_CHECK_CLOSE(y, 2.0, 0.00001);
            BOOST_CHECK_CLOSE(z, 3.0, 0.00001);
            BOOST_CHECK_EQUAL(t, i+7);

        }
    }

    delete iter;

    return;
}


BOOST_AUTO_TEST_CASE(test_random_mode)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 102.0, 103.0);
    pdal::drivers::faux::Reader reader(bounds, 1000, pdal::drivers::faux::Reader::Random);
    reader.initialize();

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 750);

    StageSequentialIterator* iter = reader.createSequentialIterator();
    boost::uint32_t numRead = iter->read(data);

    BOOST_CHECK_EQUAL(numRead, 750u);

    int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    for (boost::uint32_t i=0; i<numRead; i++)
    {
        double x = data.getField<double>(i, offsetX);
        double y = data.getField<double>(i, offsetY);
        double z = data.getField<double>(i, offsetZ);
        boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);
        
        BOOST_CHECK_GE(x, 1.0);
        BOOST_CHECK_LE(x, 101.0);
        
        BOOST_CHECK_GE(y, 2.0);
        BOOST_CHECK_LE(y, 102.0);
        
        BOOST_CHECK_GE(z, 3.0);
        BOOST_CHECK_LE(z, 103.0);
        
        BOOST_CHECK_EQUAL(t, i);
        // BOOST_CHECK(x >= 1.0 && x <= 101.0);
        // BOOST_CHECK(y >= 2.0 && y <= 102.0);
        // BOOST_CHECK(z >= 3.0 && z <= 103.0);
        // BOOST_CHECK(t == i);
    }

    delete iter;

    return;
}

BOOST_AUTO_TEST_CASE(test_ramp_mode_1)
{
    Bounds<double> bounds(0,0,0,4,4,4);
    pdal::drivers::faux::Reader reader(bounds, 2, pdal::drivers::faux::Reader::Ramp);
    reader.initialize();

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 2);

    StageSequentialIterator* iter = reader.createSequentialIterator();
    boost::uint32_t numRead = iter->read(data);

    BOOST_CHECK_EQUAL(numRead, 2u);

    const int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    const int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    const int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    const int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    const double x0 = data.getField<double>(0, offsetX);
    const double y0 = data.getField<double>(0, offsetY);
    const double z0 = data.getField<double>(0, offsetZ);
    const boost::uint64_t t0 = data.getField<boost::uint64_t>(0, offsetT);

    const double x1 = data.getField<double>(1, offsetX);
    const double y1 = data.getField<double>(1, offsetY);
    const double z1 = data.getField<double>(1, offsetZ);
    const boost::uint64_t t1 = data.getField<boost::uint64_t>(1, offsetT);

    BOOST_CHECK_CLOSE(x0, 0.0, 0.00001);
    BOOST_CHECK_CLOSE(y0, 0.0, 0.00001);
    BOOST_CHECK_CLOSE(z0, 0.0, 0.00001);
    BOOST_CHECK_EQUAL(t0, 0);

    BOOST_CHECK_CLOSE(x1, 4.0, 0.00001);
    BOOST_CHECK_CLOSE(y1, 4.0, 0.00001);
    BOOST_CHECK_CLOSE(z1, 4.0, 0.00001);
    BOOST_CHECK_EQUAL(t1, 1);


    delete iter;

    return;
}


BOOST_AUTO_TEST_CASE(test_ramp_mode_2)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 152.0, 203.0);
    pdal::drivers::faux::Reader reader(bounds, 750, pdal::drivers::faux::Reader::Ramp);
    reader.initialize();

    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 750);

    StageSequentialIterator* iter = reader.createSequentialIterator();
    boost::uint32_t numRead = iter->read(data);

    BOOST_CHECK_EQUAL(numRead,750u);

    int offsetX = schema.getDimensionIndex(Dimension::Id_X_f64);
    int offsetY = schema.getDimensionIndex(Dimension::Id_Y_f64);
    int offsetZ = schema.getDimensionIndex(Dimension::Id_Z_f64);
    int offsetT = schema.getDimensionIndex(Dimension::Id_Time_u64);

    double delX = (101.0 - 1.0) / (750.0 - 1.0);
    double delY = (152.0 - 2.0) / (750.0 - 1.0);
    double delZ = (203.0 - 3.0) / (750.0 - 1.0);

    for (boost::uint32_t i=0; i<numRead; i++)
    {
        double x = data.getField<double>(i, offsetX);
        double y = data.getField<double>(i, offsetY);
        double z = data.getField<double>(i, offsetZ);
        boost::uint64_t t = data.getField<boost::uint64_t>(i, offsetT);

        BOOST_CHECK_CLOSE(x, 1.0 + delX*i, 0.00001);
        BOOST_CHECK_CLOSE(y, 2.0 + delY*i, 0.00001);
        BOOST_CHECK_CLOSE(z, 3.0 + delZ*i, 0.00001);
        BOOST_CHECK_EQUAL(t, i);

    }

    delete iter;

    return;
}


BOOST_AUTO_TEST_CASE(test_custom_fields)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 102.0, 103.0);

    Dimension dimY(Dimension::Id_Red_u8);
    Dimension dimX(Dimension::Id_Blue_u8);
    std::vector<Dimension> dims;
    dims.push_back(dimY);
    dims.push_back(dimX);

    pdal::drivers::faux::Reader reader(bounds, 1000, pdal::drivers::faux::Reader::Random, dims);
    reader.initialize();

    const Schema& schema = reader.getSchema();
    BOOST_CHECK_EQUAL(schema.getDimensions().size(), 2u);
    BOOST_CHECK_EQUAL(schema.getDimension(0).getId(), Dimension::Id_Red_u8);
    BOOST_CHECK_EQUAL(schema.getDimension(1).getId(), Dimension::Id_Blue_u8);

    return;
}



BOOST_AUTO_TEST_CASE(test_iterator_checks)
{
    Bounds<double> bounds(1.0, 2.0, 3.0, 101.0, 152.0, 203.0);
    pdal::drivers::faux::Reader reader(bounds, 750, pdal::drivers::faux::Reader::Ramp);
    reader.initialize();
    
    const Schema& schema = reader.getSchema();
    SchemaLayout layout(schema);

    PointBuffer data(layout, 750);
    
    
    BOOST_CHECK_EQUAL(reader.supportsIterator(StageIterator_Sequential), true);
    BOOST_CHECK_EQUAL(reader.supportsIterator(StageIterator_Random) , true);

    return;
}

BOOST_AUTO_TEST_SUITE_END()
