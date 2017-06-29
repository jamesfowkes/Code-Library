#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "lib_i2c_common.h"

/*
 * Defines and Typedefs
 */


/* 
 * Private Function Prototypes
 */

//static void rd_callback(I2C_TRANSFER_DATA * transfer);
static void wr_callback(I2C_TRANSFER_DATA * transfer);

/* 
 * Local Variables
 */

static I2C_TRANSFER_DATA s_i2c_transfer;
static uint8_t s_mr_data_sent[10];
static uint8_t s_sent_count;
static uint8_t s_mr_data_recvd[10];
static uint8_t s_recv_count;

class I2CTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(I2CTest);
	CPPUNIT_TEST(test_MR);
	CPPUNIT_TEST_SUITE_END();

	void test_MR(void)
	{
		s_i2c_transfer.buffer = s_mr_data_recvd;
		s_i2c_transfer.totalBytes = sizeof(s_mr_data_recvd);
		s_i2c_transfer.address = 0x7F;
		s_i2c_transfer.callback = wr_callback;
		I2C_StartMaster(&s_i2c_transfer, true, false);

		CPPUNIT_ASSERT_EQUAL((uint8_t)0xFF, s_mr_data_sent[0]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)0, s_recv_count);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'A', s_mr_data_recvd[0]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'B', s_mr_data_recvd[1]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'C', s_mr_data_recvd[2]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'D', s_mr_data_recvd[3]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'E', s_mr_data_recvd[4]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'F', s_mr_data_recvd[5]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'G', s_mr_data_recvd[6]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'H', s_mr_data_recvd[7]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'I', s_mr_data_recvd[8]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)'J', s_mr_data_recvd[9]);
	}

	public:
	void tearDown(void)
	{

	}

	void setUp(void)
	{
		I2C_Init(NULL);
		s_sent_count = 0;
		s_recv_count = 0;
	}
};

//static void rd_callback(I2C_TRANSFER_DATA * transfer)
//{
//	(void)transfer;
//}

static void wr_callback(I2C_TRANSFER_DATA * transfer)
{
	(void)transfer;
}

void I2C_PrivateInit(void)
{

}

void start(void)
{
	I2C_New_Event(I2C_START);
}

void ack(void) { I2C_New_Event(I2C_DATA_ACK); }

void send(void)
{
	I2C_New_Event(s_sent_count < 10 ? I2C_SLA_ACK : I2C_SLA_NACK);
}

void nack(void) { I2C_New_Event(I2C_DATA_NACK); }
void stop(void)  {}
void release(void) {}

void setThisSlaveAddress(uint8_t addr) { (void)addr; }
void readData(uint8_t * data)
{
	static uint8_t next = 'A';
	*data = next++;
}

void setData(uint8_t * data)
{
	s_mr_data_sent[s_sent_count++] = *data;
}

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( I2CTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
