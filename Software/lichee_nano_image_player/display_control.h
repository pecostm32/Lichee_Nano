//--------------------------------------------------------------------------------------

#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

//--------------------------------------------------------------------------------------
//LCD timing control registers
#define TCON_CTRL              ((volatile uint32 *)(0x01C0C000))
#define TCON_INT0              ((volatile uint32 *)(0x01C0C004))
#define TCON_INT1              ((volatile uint32 *)(0x01C0C008))

//TCON0 registers are for the LCD output channel
#define TCON0_CTRL             ((volatile uint32 *)(0x01C0C040))
#define TCON_CLK_CTRL          ((volatile uint32 *)(0x01C0C044))
#define TCON0_BASIC_TIMING0    ((volatile uint32 *)(0x01C0C048))
#define TCON0_BASIC_TIMING1    ((volatile uint32 *)(0x01C0C04C))
#define TCON0_BASIC_TIMING2    ((volatile uint32 *)(0x01C0C050))
#define TCON0_BASIC_TIMING3    ((volatile uint32 *)(0x01C0C054))
#define TCON0_HV_TIMING        ((volatile uint32 *)(0x01C0C058))
#define TCON0_CPU_IF           ((volatile uint32 *)(0x01C0C060))

#define TCON0_IO_CTRL0         ((volatile uint32 *)(0x01C0C088))
#define TCON0_IO_CTRL1         ((volatile uint32 *)(0x01C0C08C))

//TCNO1 registers are for the TV output channel
#define TCON1_IO_CTRL1         ((volatile uint32 *)(0x01C0C0F4))

//--------------------------------------------------------------------------------------
//Display engine front end registers
#define DEFE_EN_REG            ((volatile uint32 *)(0x01E00000))
#define DEFE_FRM_CTRL_REG      ((volatile uint32 *)(0x01E00004))
#define DEFE_BYPASS_REG        ((volatile uint32 *)(0x01E00008))
#define DEFE_AGTH_SEL_REG      ((volatile uint32 *)(0x01E0000C))
#define DEFE_LINT_CTRL_REG     ((volatile uint32 *)(0x01E00010))
#define DEFE_BUF_ADDR0_REG     ((volatile uint32 *)(0x01E00020))
#define DEFE_BUF_ADDR1_REG     ((volatile uint32 *)(0x01E00024))
#define DEFE_BUF_ADDR2_REG     ((volatile uint32 *)(0x01E00028))
#define DEFE_FIELD_CTRL_REG    ((volatile uint32 *)(0x01E0002C))
#define DEFE_TB_OFF0_REG       ((volatile uint32 *)(0x01E00030))
#define DEFE_TB_OFF1_REG       ((volatile uint32 *)(0x01E00034))
#define DEFE_TB_OFF2_REG       ((volatile uint32 *)(0x01E00038))
#define DEFE_LINESTRD0_REG     ((volatile uint32 *)(0x01E00040))
#define DEFE_LINESTRD1_REG     ((volatile uint32 *)(0x01E00044))
#define DEFE_LINESTRD2_REG     ((volatile uint32 *)(0x01E00048))
#define DEFE_INPUT_FMT_REG     ((volatile uint32 *)(0x01E0004C))
#define DEFE_WB_ADDR_REG       ((volatile uint32 *)(0x01E00050))
#define DEFE_OUTPUT_FMT_REG    ((volatile uint32 *)(0x01E0005C))
#define DEFE_INT_EN_REG        ((volatile uint32 *)(0x01E00060))
#define DEFE_INT_STATUS_REG    ((volatile uint32 *)(0x01E00064))
#define DEFE_STATUS_REG        ((volatile uint32 *)(0x01E00068))
#define DEFE_CSC_COEF00_REG    ((volatile uint32 *)(0x01E00070))
#define DEFE_CSC_COEF01_REG    ((volatile uint32 *)(0x01E00074))
#define DEFE_CSC_COEF02_REG    ((volatile uint32 *)(0x01E00078))
#define DEFE_CSC_COEF03_REG    ((volatile uint32 *)(0x01E0007C))
#define DEFE_CSC_COEF10_REG    ((volatile uint32 *)(0x01E00080))
#define DEFE_CSC_COEF11_REG    ((volatile uint32 *)(0x01E00084))
#define DEFE_CSC_COEF12_REG    ((volatile uint32 *)(0x01E00088))
#define DEFE_CSC_COEF13_REG    ((volatile uint32 *)(0x01E0008C))
#define DEFE_CSC_COEF20_REG    ((volatile uint32 *)(0x01E00090))
#define DEFE_CSC_COEF21_REG    ((volatile uint32 *)(0x01E00094))
#define DEFE_CSC_COEF22_REG    ((volatile uint32 *)(0x01E00098))
#define DEFE_CSC_COEF23_REG    ((volatile uint32 *)(0x01E0009C))

#define DEFE_WB_LINESTRD_EN_REG  ((volatile uint32 *)(0x01E000D0))
#define DEFE_WB_LINESTRD_REG     ((volatile uint32 *)(0x01E000D4))

#define DEFE_INSIZE_REG        ((volatile uint32 *)(0x01E00100))
#define DEFE_OUTSIZE_REG       ((volatile uint32 *)(0x01E00104))
#define DEFE_HORZFACT_REG      ((volatile uint32 *)(0x01E00108))
#define DEFE_VERTFACT_REG      ((volatile uint32 *)(0x01E0010C))

//Coefficient registers are 32 word arrays
#define DEFE_CH0_HORZCOEF_REGN ((volatile uint32 *)(0x01E00400))
#define DEFE_CH0_VERTCOEF_REGN ((volatile uint32 *)(0x01E00500))
#define DEFE_CH1_HORZCOEF_REGN ((volatile uint32 *)(0x01E00600))
#define DEFE_CH1_VERTCOEF_REGN ((volatile uint32 *)(0x01E00700))

//--------------------------------------------------------------------------------------
//Display engine back end registers
#define DEBE_MODE_CTRL         ((volatile uint32 *)(0x01E60800))
#define DEBE_BACKGROUND_COLOR  ((volatile uint32 *)(0x01E60804))

#define DEBE_LAY_SIZE          ((volatile uint32 *)(0x01E60808))    //Not present in the manual but used in the scope software and set to the same value as DEBE_LAY0_SIZE

#define DEBE_LAY0_SIZE         ((volatile uint32 *)(0x01E60810))
#define DEBE_LAY1_SIZE         ((volatile uint32 *)(0x01E60814))
#define DEBE_LAY2_SIZE         ((volatile uint32 *)(0x01E60818))
#define DEBE_LAY3_SIZE         ((volatile uint32 *)(0x01E6081C))

#define DEBE_LAY0_LINEWIDTH    ((volatile uint32 *)(0x01E60840))
#define DEBE_LAY1_LINEWIDTH    ((volatile uint32 *)(0x01E60844))
#define DEBE_LAY2_LINEWIDTH    ((volatile uint32 *)(0x01E60848))
#define DEBE_LAY3_LINEWIDTH    ((volatile uint32 *)(0x01E6084C))

#define DEBE_LAY0_FB_ADDR1L    ((volatile uint32 *)(0x01E60850))
#define DEBE_LAY0_FB_ADDR2L    ((volatile uint32 *)(0x01E60854))
#define DEBE_LAY0_FB_ADDR3L    ((volatile uint32 *)(0x01E60858))
#define DEBE_LAY0_FB_ADDR4L    ((volatile uint32 *)(0x01E6085C))

#define DEBE_LAY0_FB_ADDR1H    ((volatile uint32 *)(0x01E60860))    //Not present in the manual but used in the scope to set the top 3 bits of the frame buffer address
#define DEBE_LAY0_FB_ADDR2H    ((volatile uint32 *)(0x01E60864))
#define DEBE_LAY0_FB_ADDR3H    ((volatile uint32 *)(0x01E60868))
#define DEBE_LAY0_FB_ADDR4H    ((volatile uint32 *)(0x01E6086C))


#define DEBE_REGBUFF_CTRL      ((volatile uint32 *)(0x01E60870))

#define DEBE_LAY0_ATT_CTRL1    ((volatile uint32 *)(0x01E608A0))

#define DEBE_COEF23            ((volatile uint32 *)(0x01E6097C))

//--------------------------------------------------------------------------------------
//LCD timing control settings
#define TCON_CTRL_MODULE_EN                 0x80000000         //Enable the LCD timing control module
#define TCON_CTRL_IO_MAP_SEL_TCON1          0x00000001         //Use the TCON1 registers instead of the TCON0 registers



//--------------------------------------------------------------------------------------
//Display engine front end settings
#define DEFE_EN_ENABLE                      0x00000001         //Enable the DEFE part
#define DEFE_EN_BIST_EN                     0x80000000         //Enable DEFE full access to the ram

#define DEFE_FRM_CTRL_COEF_ACCESS           0x00800000         //Allow the CPU to access the coefficient memory
#define DEFE_FRM_CTRL_FRAME_START           0x00010000         //To start processing a new frame

#define DEFE_FRM_CTRL_OUT_CTRL              0x00000800         //Disable DEFE otuput to image (DEBE??)

#define DEFE_FRM_CTRL_WB_EN                 0x00000004         //Enable write back function
#define DEFE_FRM_CTRL_REG_RDY_EN            0x00000000         //Signal DEFE that new settings are ready to be used for next frame

#define DEFE_BYPASS_CSC_BYPASS_EN           0x00000002         //Color space converter will not be used when bit set
#define DEFE_BYPASS_DEFE_BYPASS_EN          0x00000001         //Scaling will not be done when bit is set


#define DEFE_AGTH_SEL_4_4                   0x00000000
#define DEFE_AGTH_SEL_4_2                   0x00000001
#define DEFE_AGTH_SEL_4_1                   0x00000002         //For horizontal scaling only use this one??

#define DEFE_LINT_CTRL_CURRENT_LINE         0x0FFF0000         //Mask for reading back the current line being worked on
#define DEFE_LINT_CTRL_FIELD_SELECT         0x00008000         //Select end field????
#define DEFE_LINT_CTRL_TRIG_LINE            0x00001FFF         //Line that interrupt will be generated on???


#define DEFE_INPUT_FMT_BYTE_SEQ             0x00010000         //To inverse the byte order of how the data is given
#define DEFE_INPUT_FMT_SCAN_MOD             0x00001000         //Set for interlaced. Not sure how this works in relation to in and out
#define DEFE_INPUT_FMT_DATA_MOD_NT_P        0x00000000         //Mode to be used
#define DEFE_INPUT_FMT_DATA_MOD_NT_I        0x00000100
#define DEFE_INPUT_FMT_DATA_MOD_NT_UV       0x00000200
#define DEFE_INPUT_FMT_DATA_MOD_T_P         0x00000400
#define DEFE_INPUT_FMT_DATA_MOD_T_UV        0x00000600

#define DEFE_INPUT_FMT_YUV_4_4_4            0x00000000
#define DEFE_INPUT_FMT_YUV_4_2_2            0x00000010
#define DEFE_INPUT_FMT_YUV_4_2_0            0x00000020
#define DEFE_INPUT_FMT_YUV_4_1_1            0x00000030
#define DEFE_INPUT_FMT_CSI_RGB              0x00000040
#define DEFE_INPUT_FMT_RGB888               0x00000050

#define DEFE_INPUT_FMT_DATA_PS              0x00000003        //Mask for the pixel sequence. Probably not needed


#define DEFE_STATUS_WB_STATUS               0x00000002

//--------------------------------------------------------------------------------------
//Display engine back end settings
#define DEBE_MODE_CTRL_EN                   0x00000001         //Enable the display engine back end
#define DEBE_MODE_CTRL_START                0x00000002         //Start the display engine back end

#define DEBE_MODE_CTRL_LAYER0_EN            0x00000100         //Enable layer0



#define DEBE_REGBUFF_CTRL_LOAD              0x00000001         //Load the module registers



#define DEBE_LAY0_ATT_CTRL1_RGB565          0x00000500         //16 bits per pixel RGB 565 mode


#define DEBE_NUMBER_OF_BYTES                384

//--------------------------------------------------------------------------------------
//Functions

void sys_disable_display(void);

void sys_init_display(uint16 xsize, uint16 ysize, uint16 *address);

void display_bitmap(uint16 xpos, uint16 ypos, uint16 xsize, uint16 ysize, uint16 *source, uint16 *dest);

//--------------------------------------------------------------------------------------

#endif /* DISPLAY_CONTROL_H */

