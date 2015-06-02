
/***************************** Include Files *********************************/
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include <c++/4.8.3/vector>
#include <c++/4.8.3/algorithm>

#if (!defined(DEBUG))
extern void xil_printf(const char *format, ...);
#endif

/******************** Constant Definitions **********************************/

/*
 * Device hardware build related constants.
 */


#define DMA_DEV_ID		XPAR_AXIDMA_1_DEVICE_ID

#define MEM_BASE_0_ADDR		0x10000000
#define MEM_BASE_1_ADDR		0x10000000 + 0x00500000
//#define ACC_ADDR 			XPAR_CL_ACCELERATOR_1_BASEADDR
//
//#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR)
//#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00000FFF)
//#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00001000)
//#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00001FFF)
//#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
//#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
//#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)



using vec_t = std::vector<float>;
using vec_it = std::vector<float>::iterator;
using vec_c_it = std::vector<float>::const_iterator;


struct ConvLayerValues {
	vec_c_it image;
	vec_c_it weights;
	const int img_dim;
	const int kernel_dim;
	const float bias;
	const float avg_pool_coefficient;
	const float avg_pool_bias;
	const float scale_factor;
	vec_it feature_map;
};

struct buffer_addr {
	int mem_base_addr;
	const int tx_bd_space_base = mem_base_addr;
	const int tx_bd_space_high = mem_base_addr +0x00000FFF;
	const int rx_bd_space_base = mem_base_addr + 0x00001000;
	const int rx_bd_space_high = mem_base_addr + 0x00001FFF;
	const int tx_buffer_base = mem_base_addr + 0x00100000;
	const int rx_buffer_base = mem_base_addr + 0x00300000;
	const int rx_buffer_high = mem_base_addr + 0x004FFFFF;

};

using vec_clv = std::vector<ConvLayerValues>;
using feature_map_parameters = std::vector<vec_clv>;



class ClAccDriver {
public:
	ClAccDriver();
	void CalculateLayer(feature_map_parameters &fmp);

private:
	std::vector<int> dma_ids = {XPAR_AXIDMA_0_DEVICE_ID, XPAR_AXIDMA_1_DEVICE_ID};
	std::vector<int> acc_addr = {XPAR_CL_ACCELERATOR_0_BASEADDR, XPAR_CL_ACCELERATOR_1_BASEADDR};
	std::vector<buffer_addr> dma_buffer_addr;

	int TransferDatatoAccAndSetupRx(const std::vector<ConvLayerValues> &clv_vec);
	void ConfigureAndRunAccelerator(int nof_outputs, int layer, int nof_sets);
	void WriteDataToTxBuffer(const std::vector<ConvLayerValues> &clv_vec);
	int RxSetup(XAxiDma * AxiDmaInstPtr, const int recv_length);
	int TxSetup(XAxiDma * AxiDmaInstPtr);
	int SendPacket(XAxiDma * AxiDmaInstPtr, const std::vector<ConvLayerValues> &clv_vec);
	int GetDataFromRxBuffer(vec_it iterator, int data_size);
	int WaitForTxToFinish(XAxiDma * AxiDmaInstPtr);
	int WaitForRxToFinish(XAxiDma * AxiDmaInstPtr);

};


