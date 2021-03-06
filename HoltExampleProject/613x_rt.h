/* ----------------------------------------------------------------------------
 *                            HOLT Integrated Circuits 
 * ----------------------------------------------------------------------------
 *
 *    file	613x_rt.h
 *    brief     This file contains prototype functions and
 * 	        definitions used by functions in 613x_rt.c file. 
 *
 *		IMPORTANT: Edit file 613x_initialization.h to define either:
 *
 *
 *		Most functional procedures in this project are written for
 *		both parallel bus or SPI host interface schemes, this runtime 
 *		switch determines which are procedures are compiled.
 * 
 *
 *	   	HOLT DISCLAIMER
 *      	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
 *      	KIND, EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
 *      	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 *      	PURPOSE AND NONINFRINGEMENT. 
 *      	IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *      	OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *      	OTHERWISE,ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *
 *              Copyright (C) 2009-2011 by  HOLT, Inc.
 *              All Rights Reserved
 */

//------------------------------------------------------------------------
//                            Macro definitions
//------------------------------------------------------------------------

// These tables are relocatable...
#define RT1_ILLEGAL_TABLE_BASE_ADDR 0x200 // RT1 default illegalization table addr range 0x0200 to 0x02FF
#define RT2_ILLEGAL_TABLE_BASE_ADDR 0x300 // RT2 default illegalization table addr range 0x0300 to 0x03FF
#define RT1_DESCRIP_TABLE_BASE_ADDR 0x400 // RT1 default descriptor table addr range 0x0400 to 0x05FF
#define RT2_DESCRIP_TABLE_BASE_ADDR 0x600 // RT2 default descriptor table addr range 0x0600 to 0x07FF




//--------------------------------------------------------------------------------------
//
//         H O S T   B U S   I N T E R F A C E   H I - 6 1 3 0   O N L Y
//
//   ( H O S T   S P I   I N T E R F A C E   H I - 6 1 3 1   S E C T I O N   B E L O W )
//
//---------------------------------------------------------------------------------------


// THIS SECTION ONLY APPLIES TO THE MEMORY-MAPPED HI-6130. IT *DOES NOT* APPLY TO HI-6131.
                                                                                               
// The HI-613x has up to two Remote Terminals. Here are macros and structures that simplify
// addressing for the RT1 and RT2 Descriptor Tables and command Illegalization Tables for 
// HI-6130 (HOST_BUS_INTERFACE) only. The HI-6131 (SPI INTERFACE) is not memory-mapped so 
// other addressing methods are used. Far below, examples demonstrate HI-6130 bus addressing 
// using these structures. Use of these structures is optional. For example, the functions 
// that load demo initialization values into RAM do so without using these structs. 
                                                                                             

//--------------------------------------------------------------------------
//  DESCRIPTOR TABLE: NESTED STRUCTURE HEIRARCHY FOR HI-6130 BUS ADDRESSING
//--------------------------------------------------------------------------

//                                ___________________ chip select CS0 base address for ARM Cortex M3
//                               |
//                               |                 _______ table base address is doubled for bus addressing
//                               |                |
#define RT1_DTABLE_BUS_ADDR (0x60000000 + (RT1_DESCRIP_TABLE_BASE_ADDR << 1))  
#define RT2_DTABLE_BUS_ADDR (0x60000000 + (RT2_DESCRIP_TABLE_BASE_ADDR << 1))  


// starting with the most elementary structure

struct dTableBlock {
  // building block only, no instances declared...
  volatile unsigned short ctrlWord;
  volatile unsigned short descWord2;
  volatile unsigned short descWord3;
  volatile unsigned short descWord4;
} ;

struct saQuad { 
  // subaddress table quadrant, Ax = subaddress #
  // building block only, no instances declared...
  struct dTableBlock A0;
  struct dTableBlock A1;
  struct dTableBlock A2;
  struct dTableBlock A3;
  struct dTableBlock A4;
  struct dTableBlock A5;
  struct dTableBlock A6;
  struct dTableBlock A7;
  struct dTableBlock A8;
  struct dTableBlock A9;
  struct dTableBlock A10;
  struct dTableBlock A11;
  struct dTableBlock A12;
  struct dTableBlock A13;
  struct dTableBlock A14;
  struct dTableBlock A15;
  struct dTableBlock A16;
  struct dTableBlock A17;
  struct dTableBlock A18;
  struct dTableBlock A19;
  struct dTableBlock A20;
  struct dTableBlock A21;
  struct dTableBlock A22;
  struct dTableBlock A23;
  struct dTableBlock A24;
  struct dTableBlock A25;
  struct dTableBlock A26;
  struct dTableBlock A27;
  struct dTableBlock A28;
  struct dTableBlock A29;
  struct dTableBlock A30;
  struct dTableBlock A31;
} ;

struct mcQuad {
  // mode code table quadrant, Cx = mode code #
  // building block only, no instances declared...
  struct dTableBlock C0;
  struct dTableBlock C1;
  struct dTableBlock C2;
  struct dTableBlock C3;
  struct dTableBlock C4;
  struct dTableBlock C5;
  struct dTableBlock C6;
  struct dTableBlock C7;
  struct dTableBlock C8;
  struct dTableBlock C9;
  struct dTableBlock C10;
  struct dTableBlock C11;
  struct dTableBlock C12;
  struct dTableBlock C13;
  struct dTableBlock C14;
  struct dTableBlock C15;
  struct dTableBlock C16;
  struct dTableBlock C17;
  struct dTableBlock C18;
  struct dTableBlock C19;
  struct dTableBlock C20;
  struct dTableBlock C21;
  struct dTableBlock C22;
  struct dTableBlock C23;
  struct dTableBlock C24;
  struct dTableBlock C25;
  struct dTableBlock C26;
  struct dTableBlock C27;
  struct dTableBlock C28;
  struct dTableBlock C29;
  struct dTableBlock C30;
  struct dTableBlock C31;
} ;

#if(RT1_ena)
typedef struct rt1_d_table {
  // the top level Descriptor Table structure, we declare 
  // instances for RT1 and its pointer
  struct saQuad RxS;
  struct saQuad TxS;
  struct mcQuad RxM;
  struct mcQuad TxM;
} rt1_desc_table, *RT1d;

#define RT1_DTABLE_BASE	((RT1d) RT1_DTABLE_BUS_ADDR) 	

#endif // (RT1_ena)

#if(RT2_ena)
typedef struct rt2_d_table {
  // the top level Descriptor Table structure, we declare 
  // instances for RT1 and its pointer
  struct saQuad RxS;
  struct saQuad TxS;
  struct mcQuad RxM;
  struct mcQuad TxM;
} rt2_desc_table, *RT2d;

#define RT2_DTABLE_BASE	((RT2d) RT2_DTABLE_BUS_ADDR) 	

#endif // (RT2_ena)


//------------------------------------------------------------------------------
//  ILLEGALIZATION TABLE: NESTED STRUCTURE HEIRARCHY FOR HI-6130 BUS ADDRESSING
//------------------------------------------------------------------------------

//                                ___________________ chip select CS0 base address for ARM Cortex M3
//                               |
//                               |                 _______ table base address is doubled for bus addressing
//                               |                |
#define RT1_ITABLE_BUS_ADDR (0x60000000 + (RT1_ILLEGAL_TABLE_BASE_ADDR << 1))  
#define RT2_ITABLE_BUS_ADDR (0x60000000 + (RT2_ILLEGAL_TABLE_BASE_ADDR << 1))  

// starting with the most elementary structure

struct num_word {
  // building block only, no instances declared...
  // 32 single-bit "word count" fields spanning 2 16-bit words
  unsigned _32words : 1;
  unsigned _1word : 1;
  unsigned _2words : 1;
  unsigned _3words : 1;
  unsigned _4words : 1;
  unsigned _5words : 1;
  unsigned _6words : 1;
  unsigned _7words : 1;
  unsigned _8words : 1;
  unsigned _9words : 1;
  unsigned _10words : 1;
  unsigned _11words : 1;
  unsigned _12words : 1;
  unsigned _13words : 1;
  unsigned _14words : 1;
  unsigned _15words : 1;
  unsigned _16words : 1;
  unsigned _17words : 1;
  unsigned _18words : 1;
  unsigned _19words : 1;
  unsigned _20words : 1;
  unsigned _21words : 1;
  unsigned _22words : 1;
  unsigned _23words : 1;
  unsigned _24words : 1;
  unsigned _25words : 1;
  unsigned _26words : 1;
  unsigned _27words : 1;
  unsigned _28words : 1;
  unsigned _29words : 1;
  unsigned _30words : 1;
  unsigned _31words : 1;
} ;

  
struct code_num {
  // building block only, no instances declared...
  // 32 single-bit "mode code #" fields spanning 2 16-bit words
  unsigned _mcode0 : 1;
  unsigned _mcode1 : 1;
  unsigned _mcode2 : 1;
  unsigned _mcode3 : 1;
  unsigned _mcode4 : 1;
  unsigned _mcode5 : 1;
  unsigned _mcode6 : 1;
  unsigned _mcode7 : 1;
  unsigned _mcode8 : 1;
  unsigned _mcode9 : 1;
  unsigned _mcode10 : 1;
  unsigned _mcode11 : 1;
  unsigned _mcode12 : 1;
  unsigned _mcode13 : 1;
  unsigned _mcode14 : 1;
  unsigned _mcode15 : 1;
  unsigned _mcode16 : 1;
  unsigned _mcode17 : 1;
  unsigned _mcode18 : 1;
  unsigned _mcode19 : 1;
  unsigned _mcode20 : 1;
  unsigned _mcode21 : 1;
  unsigned _mcode22 : 1;
  unsigned _mcode23 : 1;
  unsigned _mcode24 : 1;
  unsigned _mcode25 : 1;
  unsigned _mcode26 : 1;
  unsigned _mcode27 : 1;
  unsigned _mcode28 : 1;
  unsigned _mcode29 : 1;
  unsigned _mcode30 : 1;
  unsigned _mcode31 : 1;
} ;

 
struct itQuad { 
  // table quadrant. SA0 = SA31 = mode code.
  // SA1 through SA30 = subaddress Tx or Rx commands.
  // building block only, no instances declared...
  struct code_num SA0;
  struct num_word SA1;
  struct num_word SA2;
  struct num_word SA3;
  struct num_word SA4;
  struct num_word SA5;
  struct num_word SA6;
  struct num_word SA7;
  struct num_word SA8;
  struct num_word SA9;
  struct num_word SA10;
  struct num_word SA11;
  struct num_word SA12;
  struct num_word SA13;
  struct num_word SA14;
  struct num_word SA15;
  struct num_word SA16;
  struct num_word SA17;
  struct num_word SA18;
  struct num_word SA19;
  struct num_word SA20;
  struct num_word SA21;
  struct num_word SA22;
  struct num_word SA23;
  struct num_word SA24;
  struct num_word SA25;
  struct num_word SA26;
  struct num_word SA27;
  struct num_word SA28;
  struct num_word SA29;
  struct num_word SA30;
  struct code_num SA31;
};


#if(RT1_ena)
typedef struct rt1_illcmd_table {
  // here is the top level Illegalization Table structure, 
  // we declare instances for RT1 and its pointer
  struct itQuad BRx;  // broadcast receive
  struct itQuad BTx;  // broadcast transmit
  struct itQuad Rx;   // non-broadcast receive
  struct itQuad Tx;   // non-broadcast transmit
} rt1_illegal_table, *RT1i;

#define RT1_ITABLE_BASE	((RT1i) RT1_ITABLE_BUS_ADDR) 	

#endif // (RT1_ena)


#if(RT2_ena)
typedef struct rt2_illcmd_table {
  // here is the top level Illegalization Table structure, 
  // we declare instances for RT2 and its pointer
  struct itQuad BRx;  // broadcast receive
  struct itQuad BTx;  // broadcast transmit
  struct itQuad Rx;   // non-broadcast receive
  struct itQuad Tx;   // non-broadcast transmit
} rt2_illegal_table, *RT2i;

#define RT2_ITABLE_BASE	((RT2i) RT2_ITABLE_BUS_ADDR) 	

#endif // (RT2_ena)



/*----------------------------------------------------------------------------
  HI-6130 bus addressing examples for RT1 or RT2 command Illegalization Table
------------------------------------------------------------------------------

This declaration must be within scope:   const RT1i pRT1i = RT1_ITABLE_BASE; // when using RT1
                                         const RT2i pRT2i = RT2_ITABLE_BASE; // when using RT2

Subaddress and mode code values are decimal, 0-31


        ___________ pRT1i (for RT1 command illegalization table) or
       |            pRT2i (for RT2 command illegalizationtable)
       |
       |     __________ BRx or BTx (for broadcast Rx or broadcast Tx commands) or 
       |    |           Rx  or Tx (for non-broadcast Rx or non-broadcast Tx commands)
       |    |
       |    |     _________ SA0 or SA31 *ONLY* if mode code (and next field MUST be "mcode"), or 
       |    |    |          SA1 thru SA30 inclusive if subaddress
       |    |    |
       |    |    |      _______ mcode0 thru mcode31 inclusive if mode code, or
       |    |    |     |        _1word or _2words thru _32words inclusive if subaddress
       |    |    |     |
j = pRT2i->BTx.SA0.mcode8;   // read RT2 illegal bit: broadcast Tx subaddress 0, mode code 8
k = pRT1i->Tx.SA31.mcode21;  // read RT1 illegal bit: non-broadcast Rx subaddress 31, mode code 21
i = pRT2i->BRx.SA22._1word;  // read RT2 illegal bit: for broadcast Rx subaddress 22, 1 data word
i = pRT1i->Rx.SA5._32words;  // read RT1 illegal bit: for non-broadcast Rx subaddress 5, 32 data words


------------------------------------------------------------------
  HI-6130 bus addressing examples for RT1 or RT2 Descriptor Table
------------------------------------------------------------------

This declaration must be within scope:   const RT1d pRT1d = RT1_DTABLE_BASE; // when using RT1
                                         const RT2d pRT2d = RT2_DTABLE_BASE; // when using RT2

Subaddress and mode code values are decimal, 0-31


        ___________ pRT1d (for RT1 descriptor table) or
       |            pRT2d (for RT2 descriptor table)
       |
       |     __________ RxS or TxS (for Rx or Tx subaddress) or 
       |    |           RxM or TxM (for Rx or Tx mode codes)
       |    |
       |    |    _________ A0 thru A31 if subaddress, or 
       |    |   |          C0 thru C31 if mode code
       |    |   |
       |    |   |      _______ ctrlWord, descWord2, descWord3 or descWord4
       |    |   |     |
i = pRT1d->RxS.A22.ctrlWord;   // read RT1 Control Word for Rx subaddress 22
j = pRT1d->TxS.A8.descWord3;   // read RT1 Descriptor Word 3 for Tx subaddress 8
j = pRT1d->RxM.C21.ctrlWord;   // read RT1 Control Word for Rx mode code 21
l = pRT1d->TxM.C18.descWord2;  // read RT1 Descriptor Word 2 for Tx mode code 18

The full list of RT1 descriptor control words for the 15 defined mode code commands:
rt1d->TxM->C0->ctrlWord   // defined Tx mode code 0
rt1d->TxM->C1->ctrlWord   // defined Tx mode code 1
rt1d->TxM->C2->ctrlWord   // defined Tx mode code 2
rt1d->TxM->C3->ctrlWord   // defined Tx mode code 3
rt1d->TxM->C4->ctrlWord   // defined Tx mode code 4
rt1d->TxM->C5->ctrlWord   // defined Tx mode code 5
rt1d->TxM->C6->ctrlWord   // defined Tx mode code 6
rt1d->TxM->C7->ctrlWord   // defined Tx mode code 7
rt1d->TxM->C8->ctrlWord   // defined Tx mode code 8
rt1d->TxM->C16->ctrlWord  // defined Tx mode code 16
rt1d->RxM->C17->ctrlWord  // defined Rx mode code 17
rt1d->TxM->C18->ctrlWord  // defined Tx mode code 18
rt1d->TxM->C19->ctrlWord  // defined Tx mode code 19
rt1d->RxM->C20->ctrlWord  // defined Rx mode code 20
rt1d->RxM->C21->ctrlWord  // defined Rx mode code 21


*/




//--------------------------------------------------------------------------------------
//
//         H O S T   S P I   I N T E R F A C E   H I - 6 1 3 1   O N L Y
//
//   ( H O S T   B U S   I N T E R F A C E   H I - 6 1 3 0   S E C T I O N   A B O V E )
//
//---------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//                     Function Prototypes
//-----------------------------------------------------------------------------


// self explanatory, HI-6130 ONLY
//
void RT_bus_addressing_examples(void);


//  	This function initializes the Holt HI-613x RT1 by writing 
//	configuration registers in the device. Only RT1 mode option 
//	bits are affected. The program has already called function 
// 	initialize_613x_shared() to initialize the common parameters
//	shared by BC, RT1, RT2 and/or Bus Monitor
// 
void initialize_613x_RT1(void);


//  	This function initializes the Holt HI-613x RT2 by writing 
//	configuration registers in the device. Only RT2 mode option 
//	bits are affected. The program has already called function 
// 	initialize_613x_shared() to initialize the common parameters
//	shared by BC, RT1, RT2 and/or Bus Monitor
// 
void initialize_613x_RT2(void);


//	This function checks the selected (1 or 2) RT's Status reg for RT addr parity error.	
// 
char RTAddr_okay(char RTnum);


//	This function polls pushbutton SW1 to detect/perform requested status update.	
// 
void modify_RT_status_bits(void);


// 	These functions load dummy data into the limited set of RT1 or RT2 transmit
//	buffers assigned above during initialization. This is only used for testing.
// 
void write_dummy_tx_data_RT1(void);
void write_dummy_tx_data_RT2(void);


//	This function reads BUSY and TFLAG DIP switch settings then updates HI-613x 
//      RT1 and RT2 "1553 Status Word Bits Registers" 
// 
void RTstatusUpdate(void);



// End of File

