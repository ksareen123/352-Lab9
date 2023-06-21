/*****************************************************************************
*                                                                            *
*       Source title:   Lab9_LNguyenKSareen.CPP
*                       (xxx SHORT DESCRIPTION xxx)                          *
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   03/09/17                                            *
*        Current Rev:   r.rr                                                 *
*                                                                            *
*        Description:                                                        *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*     03/09/17        - Added ALARM_DPU_DATACOUNT alarm for DPU has more     *
*                       points in named work area than captured.             *
*                       SSSR-7324) (DS)                                      *
*****************************************************************************/

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "port500d.h"
#include "cdef500d.h"
#include "util.h"
#include "gpib500d.h"
#include "config.h"
#include "testmain.h"
#include "Lab9_LNguyenKSareen.h"

/*****************************************************************************
*                                                                            *
*                         Miscellaneous (Module) #defines                    *
*                                                                            *
*****************************************************************************/

/*****************************************************************************
*                                                                            *
*                            Local (module) variables                        *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  NOTE: The Program Flow is a structure that defines the flow of the      //
//        test program. It is defined in the "\ets\inc\testmain.h" file.    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// CLab9_LNguyenKSareen
//
// The following declaration registers the test program with the test TestMain
// DLL. The value passed into the declaration specifies the Program Flow
// structure for the application. Passing in a value of ProductDatasheetFlow
// will direct the TestMain DLL to use the flow in the project's datasheet
// file.
//
CLab9_LNguyenKSareen theApp(ProductDatasheetFlow);

//{{AFX_ETS_PRODUCT_DATASHEET_VARIABLES
DECLARE_PDS_VARIABLE( PDS_INT, TestNmbr, "Test Number" );
DECLARE_PDS_VARIABLE( PDS_INT, SubTestNmbr, "Subtest Number" );
DECLARE_PDS_VARIABLE( PDS_CHAR, DLogDesc, "Datalog Description" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, HiLim, "High Limit" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, LoLim, "Low Limit" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, DFormat, "Data Format" );
DECLARE_PDS_VARIABLE( PDS_CHAR, Units, "Units" );
DECLARE_PDS_VARIABLE( PDS_CHAR, LoFBin, "Low Fail Bin" );
DECLARE_PDS_VARIABLE( PDS_CHAR, HiFBin, "High Fail Bin" );
//}}AFX_ETS_PRODUCT_DATASHEET_VARIABLES


//{{AFX_ETS_PINMAP(sites = 1)
// Caution::  The following pin definitions are filled in by the PinMap Editor.
extern "C" DLL_EXPORT int ETS_PINMAP_VERSION[] = {1,1};
extern "C" DLL_EXPORT UINT ETS_PinMap_Options = 0;
extern "C" DLL_EXPORT int ETS_PinMap_FirstSite = 0;
	DECLARE_PINMAP
		ADD_PIN (PWR, "", NAME_SP100)
		ADD_PIN (nOE, "", NAME_APU)
		ADD_PIN (DIR, "", NAME_APU)
	END_PINMAP
//}}AFX_ETS_PINMAP

//{{AFX_ETS_GROUPMAP
// Caution:: The following group definitions are filled in by the PinMap Editor
	DECLARE_GROUPMAP
		ADD_GROUP ("APU_PINS", "", APU_PINS, NAME_APU, DIR, nOE),
	END_GROUPMAP
//}}AFX_ETS_GROUPMAP
/*****************************************************************************
*                                                                            *
*                           Main-support functions                           *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserInit() function is called by the 'TestMain' module one time     //
//  near the beginning of test execution, after all Global variables and    //
//  Global Switches have been initialized.                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::UserInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Turn off the Auto Convert Units for the datasheet...
    //
    SetTestMainVariable(CONVERTUNITS, FALSE);

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for sorting datalog statements while writing results
    //              to screen/disk/printer
    //
    // Datalogging sorting modes:
    //      NO_SORT
    //      BY_TESTNUMBER (default setting)
    //      BY_PROGFLOW_NOSORT
    //      BY_PROGFLOW
    //
#if 0
    SetDlogSort( BY_TESTNUMBER);
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for loading a DSS digital vector file for use with
    // digital boards on ETS-200, ETS-300, ETS-500, ETS-564 and BT-2000 testers.
    //
    // The dssload() utility copies the vector tags from the vector file
    // into an image in computer memory.
    //
    // The utility vecload() needs to be called after dssload() to program the tag
    // information into the tester local memory.
    //
    // dssload() will not erase any vector tags in computer memory from previous
    // calls to dssload(). If there is a vector tag already in computer memory
    // and the file being loaded has a vector with the same tag, a utility error
    // will be generated and the dssload() will fail.
    //
#if 0
    if ( dssload( GetTestMainVariable(VEC_FILE)) )                      // example load vec file
    {
        etsMessageBox("Could Not Initialize Vector File !!", MB_OK | MB_ICONSTOP);
        return FALSE;   // Abort Run of xx.DLL and exit TestExecutive
    }
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for loading a DPU digital vector file for use with
    // digital boards on ETS-364 and ETS-600 testers.
    //
    // The dpinvecload() utility copies the vector patterns from the vector file
    // into an image in computer memory.
    //
#if 0
    if ( dpinvecload( GetTestMainVariable(VEC_FILE)) )                      // example load vec file
    {
        etsMessageBox("Could Not Initialize Vector File !!", MB_OK | MB_ICONSTOP);
        return FALSE;   // Abort Run of xx.DLL and exit TestExecutive
    }
#endif

    /////////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for a multisite application.
    //
    // The purpose of this function is to have the user indicate that locations
    // of the data sheet arrays to the msLogResult() and msLogResultAll()
    // functions.
    //
    // THIS FUNCTION MUST BE CALLED PRIOR TO USING THE msLogResult() AND
    // msLogResultAll() FUNCTIONS.  This function should be called in UserInit()
    //
#if 1
    msLogResultInit( TestNmbr, SubTestNmbr, DLogDesc, DFormat, LoLim, HiLim, Units, LoFBin, HiFBin );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) setup for a multisite application.
    //
    //  - First create a boolean array indicating which sites will be active.
    //  - Then call msSetSites() passing in this array.
    //
    // Additionally, ETS suppiles the FOR_EACH_SITE macro. This macro operates
    // as a C for-loop iterating through the list if valid sites in the test
    // program. This macro shown below is defined in the project's '.h' file.
    //
    // FOR_EACH_SITE( site, num_sites )
    // where:
    //         site - an integer that contains the current valid site of the
    //                iteration. During the iteration, 'site' will only contain
    //                site numbers of 'valid' test sites.
    //    num_sites - The total number of sites contained in the test program
    //
    // Example:  Log the X,Y coordinates for the test run...
    //
    //        FOR_EACH_SITE( site, total_sites )
    //        {
    //            msLogXYCoords( site, Xcoord, Ycoord );
    //        }
    //
#if 0
    int valid[NUM_SITES];
    int count;

    for( count = 0; count < NUM_SITES; count ++ )
        valid[count] = TRUE;
    msSetSites( NUM_SITES, valid );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) setup operator instructions in the test menu.
    //
    //  - Change '0' to '1' (in '#if 0' statement) if operator instructions are
    //     desired...
    //
    //  SYNTAX:
    //      SetOperatorInstruction( int linenumber, char *op_instr);
    //
    //  There are only two operator instruction lines allowed;
    //           line number can only be 1 or 2.
    //
    //  EXAMPLE:
    //      SetOperatorInstruction( 1, "Load DUT into socket and hit 'T' to test");
    //      SetOperatorInstruction( 2, "Multiple DUT test - hit F1 key");
    //
#if 0
    SetOperatorInstruction( 1, "Operator Instruction line #1" );
    SetOperatorInstruction( 2, "Operator Instruction line #2" );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) set the security access level for 'TestMain Menu' options
    //
    //  - Change '0' to '1' (in '#if 0' statement) if security access level is
    //    desired... (Options are defined in the TestMain.h file).
    //
    //    SECURITY LEVELS: 0 ( Operator ), 1 ( System Engineer )
    //
    //      OPTION#              Description                  SECURITY LEVEL (defaults)
    //      -------------------  ---------------------------  -------------------------
    //      TM_RUN               Enter Test Execution Menu    0 - Operator
    //      TM_SUMMARY           Display Summary Information  0 - Operator
    //      TM_DATASHEET         Select Device DataSheet      1 - System Engineer
    //      TM_CONTROLS          Test Control Panel           1 - System Engineer
    //      TM_TEST_SWITCHES     Test Switches Menu           1 - System Engineer
    //      TM_FMEA_SWITCHES     Failure Mode Analysis Menu   1 - System Engineer
    //      TM_USER_SWITCHES     User Switches Menu           1 - System Engineer
    //      TM_LOG_SWITCHES      Datalog Switches Menu        1 - System Engineer
    //      TM_PRINTER_SWITCHES  Printing Switches Menu       1 - System Engineer
    //      TM_DISPLAY_SWITCHES  Display Switches Menu        1 - System Engineer
    //      TM_COMPUTER_SETTINGS Computer Speed Settings Menu 1 - System Engineer
    //      TM_EXIT              Exit Test Executive          0 - Operator
    //
    //  - To allow access to items under the Test Control Panel (Test, FMEA, User,
    //    Datalog, Printer, Display or Computer Speed Menus), Access must be given to
    //    the Test Control Panel as well. These items are sub menus of the Test control
    //    menu.
    //
    //  SYNTAX:
    //    SetTestMainMenuSecurityLvl( Option#, Security_Level);
    //    * If Security_Level is 0, then Operators and System Engineers have access to
    //      the control.
    //    * If Security_Level is greater than 3, then no access is given to the
    //      particular control.
    //
    //  EXAMPLE:
    //    // Give Operator Access to User Switches menu
    //    SetTestMainMenuSecurityLvl( TM_CONTROLS, 0 );
    //    SetTestMainMenuSecurityLvl( TM_USER_SWITCHES, 0);
    //
#if 0
    SetTestMainMenuSecurityLvl( TM_CONTROLS, 1 );
#endif

    //////////////////////////////////////////////////////////////////////////
    // (Optionally) set the names of the General Purpose Switches and/or
    // Integers via a call to 'SetGlobalSwitchName()' and 'SetGlobalIntegerName()'.
    // There are 10 Global Switches and 10 Global Integers.
    //
    //  - Change '0' to '1' (in '#if 0' statement) if setting new names for
    //     the Global Switches/Integers ...
    //
    // The General Purpose Switches can be referenced by the following
    // variable names:
    //                                   Variable
    //                                     Name
    //                                   ---------
    //        General Purpose Switches: GP1  to GP10
    //        General Purpose Integers: GPI1 to GPI10
    //
    // SYNTAX:
    //   SetGlobalSwitchName( GlobalSwitch#, Name);
    //   SetGlobalIntegerName( GlobalInteger#, Name);
    //
#if 1
	SetGlobalSwitchName( GP1, "Site0");
	SetGlobalSwitchName( GP3, "Digital Functionality");
    SetGlobalSwitchName( GP9, "Leak Max" );
    SetGlobalSwitchName( GP8, "Leak High" );
	SetGlobalSwitchName( GP7, "Leak Low" );
	SetGlobalSwitchName( GP4, "Shmoo Plot Prop Delay");
	SetGlobalSwitchName( GP5, "Digital System Prop Delay");
	SetGlobalSwitchName( GP6, "Static Prop Delay");

	dpinvecload( "Lab9_LNguyenKSareen.evd" ); // load vector sheet so digital pins can use machine resources
	dpingroup("digPins", "A_PINS, B_PINS");
#endif

    //////////////////////////////////////////////////////////////////////////
    // Enables alarm checking except for Debug mode alarm checking.
    //
    // SYNTAX:
    //      int alarmset( int alarmType, int state );
    //
    // 'alarmType' valid values:
    //      ALARM_ALL         = -1 : All alarms except debug mode
    //      ALARM_DEBUGMODE   =  0 : Engineering(slow) debug mode alarms
    //      ALARM_KELVIN      =  1 : Kelvin alarms
    //      ALARM_CLAMP       =  2 : Clamp alarms
    //      ALARM_SOA         =  3 : SOA alarm, Safe Operating Range
    //      ALARM_TEMPERATURE =  4 : Temperature alarms
    //      ALARM_RAILDROOP   =  5 : Rail droop alarm
    //      ALARM_OVERRANGE   =  6 : Over Range alarms
    //      ALARM_INTERLOCK   =  7 : Interlock alarms
    //      ALARM_FATALERROR  =  8 : Fatal error alarms
    //      ALARM_VOUTERROR   =  9 : Output Voltage alarms
    //      ALARM_OVERVOLTAGETRIP = 10 : Over Voltage alarms
    //      ALARM_DPU_RCVMID  = 11 : DPU Mid-Band alarms
    //      ALARM_DPU_RCVBAD  = 12 : DPU Receive Illegal State alarm
    //      ALARM_MCU_TIMEOUT = 13 : MCU Channel timeout alarm
    //      ALARM_OVERCURRENT = 14 : Over Current alarm
    //      ALARM_PIN_MISSING = 15 : Resource pin missing on active site.
    //      ALARM_DPU_DATACOUNT = 17 : DPU more points in named work area than captured
    
    // 'state' valid values:
    //      TRUE              =  1 : Enable specified alarms
    //      FALSE             =  0 : Disable specified alarms
    //
    // Debug Alarm Checking mode will not be enabled by this call.
    // Debug Alarm Checking is meant for engineering debug work and will
    // significantly increase test time.  Debug Alarm Checking can be
    // enabled with a switch on the Testexecutive's Controls menu.
    //
    alarmset( ALARM_ALL, TRUE );

    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Loading of Test program and exit Test
    // Executive.
    //
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserExit() function is called by the 'TestMain' module one time     //
//  near the end of test execution, before the Global variables and         //
//  Global Switches are saved.                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::UserExit( void )
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestInit() function is called before the display of the Test      //
//  Main Menu to reinitialize the DUT and/or reinitialize any program       //
//  variables that might need to be reset.  This function will be invoked   //
//  prior to calling the first function in the progflow structure.          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnTestInit( void )
{
    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Entry into the Test/Quit prompt, and
    // return to Main Menu
    //
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestExit() function is called after the the selection of the      //
//  Quit button from the "Test", "Quit" Buttons Screen to reinitialize any  //
//  program variables or system resources that need to be "cleaned-up".     //
//  This function will be invoked regardless of the pass/fail status of     //
//  the DUT.                                                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnTestExit( void )
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestStartup() function is called before each test-run to          //
//  initialize any program variables or system resources that might need to //
//  be reset each time the test is run as opposed to only at the time of    //
//  program entry as in the OnTestInit() routine. This function will be     //
//  invoked prior to calling the first function in the progflow structure.  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnTestStartup( void )
{
    etsprintf( "DUT Socket Power ON\r" );

    //////////////////////////////////////////////////////////////////////////
    // Returning FALSE will abort Program flow execution and return to
    // Test/Quit prompt.
    //
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestCompletion() function is called after each test-run to        //
//  re-initialize any program variables or system resources that might need //
//  to be reset after each test suite is run This function will be invoked  //
//  after to calling the last function in the progflow structure.           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnTestCompletion( void )
{	
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );
    etsprintf( "DUT Socket Power OFF\n" );
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnLogData() function is called During a call to the LogData(),      //
//  msLogData() and msLogDataAll() functions.                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnLogData( void )
{
    int PassFail;

    if ( ( PassFail = GetTestMainVariable(PASSFAIL) ) == 2 )
    {
        //////////////////////////////////////////////////////////////////////
        // An alarm condition is present.  To determine what alarm(s) are
        // present, break on this line of code in the debugger, switch
        // to Raide and call alarmstat( 0 );
        //
        PostAlarm( "ALARM" );
    }
    else if ( PassFail )
        PostAlarm( "FAIL" );
    else
        PostAlarm( "    " );

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnFailSite() function is called During a call to the msFailSite()   //
//  to enable the user to turn off and invalidate a particular site         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnFailSite( int site )
{
	if (site == MS_SITE0) {
		apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
		apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
		dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
		sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );
		etsprintf( "DUT Socket Power OFF\n" );
	}
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnSot() function is called while waiting for a valid SOT            //
//  signal either from the Operator Box, Handler, or user interface.        //
//  SotValue contains the SOT event.                                        //
//                                                                          //
//    -2  =   Handler signaled SOT                                          //
//    -1  =   Operator Box SOT switch Pressed                               //
//    'T' =   Key Board 'T'/ or Test Button Pressed                         //
//    'Q' =   Key Board 'Q'/ or Quit Button Pressed                         //
//    F1_KEY - F5_KEY  = special Test Key/Button Pressed                    //
//    F6_KEY - F10_KEY = Datalog Mode/Binning Summary State                 //
//                       Change requested                                   //
//                                                                          //
//  The SotValue is the virtual key code of the key                         //
//  hit on the keyboard (NOT ASCII CODE). When trapping specific            //
//  key hits, use the windows defines for the virtual key codes. These      //
//  defines are: (alpha codes A thru Z)   VK_A, VK_B, ..., VK_Z             //
//               (numeric codes 0 thru 9) VK_0, VK_1, ..., VK_9             //
//               (function key codes F1 - F12) F1_KEY, F2_KEY, ..., F12_KEY //
//                                                                          //
//  Upon return from this function, the TestMain Framework will take action //
//  on the SotValue.                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

int CLab9_LNguyenKSareen::OnSot(int SotValue)
{
    return SotValue;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnUtilError function is called in response to an ETS Utility        //
//  error. The ETS Utility Error Number is passed in to the user so         //
//  certain errors can be trapped.                                          //
//                                                                          //
//  Returning TRUE will pass the error into the ETS Message Handler         //
//  program, and returning FALSE will direct the Message Handler to ignore  //
//  the error.                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnUtilError(int errnum)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  This function will automatically be called each time the interlock is   //
//  check by the Test Executive. The interlock is checked before each DUT   //
//  tested, before the call to the user's OnTestStartup() routine. The      //
//  intlocks parameter passed in is a bitmapped value containing the state  //
//  of all interlocks in the ETS Test head.                                 //
//                                                                          //
//  Returning TRUE will pass the interlock check. Returning FALSE will fail  //
//  the interlock check. If the intlocks variable is non-zero the return    //
//  value has no effect. If the interlock check fails, the DUT will not be  //
//  tested.                                                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

BOOL CLab9_LNguyenKSareen::OnInterlock(ULONG intlocks)
{
    return TRUE;
}

/*****************************************************************************
*                                                                            *
*                          Local (module) functions                          *
*                   User Specific Application Code Follows                   *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// ---------------------------[ Test Functions ]--------------------------- //
// The functions defined here are the building-blocks of the test program.  //
// Executed as ordered by the Program Flow structure array, each returns a  //
// status of SUCCESS (0) or FAILURE (-1) to indicate to the summary system  //
// whether the DUT passed each test (as controlled by the 'STOP_ON_FAIL'    //
// global switch).                                                          //
//                                                                          //
// Any (floating-point) data that is to be recorded by the datalog system   //
// is logged to, in calling-sequence order, via LogData() or msLogData()    //
// (for multisite applications) function calls.                             //
//                                                                          //
//  The function 'etsprintf()' is used to write to the TestMain Console     //
//  window. the syntax of etsprintf() is identical to C's printf(). This    //
//  function should be used if wanting to print to screen inside of test    //
//  program.                                                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////








// Test Function: Test
ETS_PRGFLOW_FUNC Test( int DSIndex, LPCTSTR TestLabel )
{
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    /////////////////////////
    // Make connections
    //     SAMPLE: hvmatonl( VinConnections );
     
    /////////////////////////
    // Apply test condition
    //     SAMPLE: fssset( GP_FSS, FSS_FV, Force[DSIndex], FSS_100MA, FSS_NOCLAMP, FSS_NOCLAMP );
     
    /////////////////////////
    // Take measurements
    //     SAMPLE: fssmi( GP_FSS, 50, 300 );
     
    /////////////////////////
    // Log the results
    msLogResultAll( DSIndex );
     
    /////////////////////////
    // Clean Up Code
    //     SAMPLE: fssset( GP_FSS, FSS_OFF, 0.0, FSS_100MA, FSS_NOCLAMP, FSS_NOCLAMP );
    //             hvmatclr();
     
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC


// Test Function: Leak_Max
ETS_PRGFLOW_FUNC Leak_Max( int DSIndex, LPCTSTR TestLabel )
{

	//trap switch
	if(!GetUserSwitch(GP9)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}

    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
	//Variable declarations
	RESULTS_STR apu_leakMH[4];
	RESULTS_STR dpin_leakMH[32];
	int i;
	
	//power IC to icc test spec into voltage mode
	sp100set( PWR, SP_FV, 5.25, SP_30V, SP_1A );

/*			IO is MAX HIGH			*/
	
	//set APU to low on each APU pin
	apuset( DIR, APU_FV, 0.0, APU_30V, APU_100MA, PIN_TO_VI );

	//disable device so buses are isolated
	apuset( nOE, APU_FV, 5.5, APU_30V, APU_100MA, PIN_TO_VI );

	//set digtal pins
	dpinviset( "digPins", DPIN_FV, 5.5, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	//wait
	lwait(1000);

	//measure leakage current from APU and log using for loop
	apumi( APU_PINS, 100, 10 );
	groupgetresults(apu_leakMH, 4);

	for(i = 0; i < 2; i++) {
		msLogResultAll(DSIndex++, apu_leakMH, 4, 1, 2); // log results one by one
	}
	

	//measure leakage current from DPU and log using for loop

	dpinmi( "digPins", 100, 10, MS_ALL );
	groupgetresults(dpin_leakMH, 32);
	for (i = 0; i < 16; i++) {
		msLogResultAll(DSIndex++, dpin_leakMH, 32, i, 16); // log results one by one
	}

	//safe shutdown
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );
	
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: Leak_High
ETS_PRGFLOW_FUNC Leak_High( int DSIndex, LPCTSTR TestLabel )
{
	//trap switch
	if(!GetUserSwitch(GP8)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}
	
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
	
	//Variable declarations
	RESULTS_STR apu_leakH[4];
	RESULTS_STR dpin_leakH[32];
	int i;
	
	//power IC to icc test spec into voltage mode
	sp100set( PWR, SP_FV, 5.25, SP_30V, SP_1A );

/*			IO is HIGH			*/
	
	//set APU to high on each APU pin
	apuset(DIR, APU_FV, 2.7, APU_30V, APU_100MA, PIN_TO_VI );

	//set enable to high
	apuset(nOE, APU_FV, 2.7, APU_30V, APU_100MA, PIN_TO_VI );

	//set digtal pins
	dpinviset( "digPins", DPIN_FV, 2.7, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	//wait
	lwait(1000);

	//measure leakage current from APU and log using for loop
	apumi( APU_PINS, 100, 10 );
	groupgetresults(apu_leakH, 4);

	for(i = 0; i < 2; i++) {
		msLogResultAll(DSIndex++, apu_leakH, 4, 1, 2); // log results one by one
	}
	
	//measure leakage current from DPU and log using for loop

	dpinmi( "digPins", 100, 10, MS_ALL );
	groupgetresults(dpin_leakH, 32);
	for (i = 0; i < 16; i++) {
		msLogResultAll(DSIndex++, dpin_leakH, 32, i, 16); // log results one by one
	}

	//safe shutdown
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );

    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: Leak_Low
ETS_PRGFLOW_FUNC Leak_Low( int DSIndex, LPCTSTR TestLabel )
{
	//trap switch
	if(!GetUserSwitch(GP7)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}

    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
//Variable declarations
	RESULTS_STR apu_leakL[1];
	RESULTS_STR apu_leakL_NOE[1];
	RESULTS_STR dpin_leakL[32];
	int i;
	
	//power IC to icc test spec into voltage mode
	sp100set( PWR, SP_FV, 5.25, SP_30V, SP_1A );

/*			IO is Low			*/
	
	//set APU to low on each APU pin
	apuset(DIR, APU_FV, 0.4, APU_30V, APU_100MA, PIN_TO_VI );

	//set enable to low
	apuset(nOE, APU_FV, 2.7, APU_30V, APU_100MA, PIN_TO_VI ); 

	//set digtal pins
	dpinviset( "digPins", DPIN_FV, 0.4, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	//wait
	lwait(1000);

	//measure leakage current from DIR
	apumi( DIR, 100, 10 );
	//etsprintf("getting DIR results");
	groupgetresults( apu_leakL, 1 );	
	//groupgetresults( apu_leakL, 4 );
	//msLogResult(DSIndex++, apu_leakL, 4, 1, 1 );
	//msLogResult( 0, DSIndex++, apu_leakL );
	msLogResultAll(DSIndex++, apu_leakL, 1, 0, 1 );

	//measure leakage current from DPU 
	dpinmi( "digPins", 100, 10, MS_ALL );
	groupgetresults(dpin_leakL, 32);
	
	//Log digpin results
	//etsprintf("getting digpin results");
	for (i = 0; i < 16; i++) {
		msLogResultAll(DSIndex++, dpin_leakL, 32, i, 16); // log results one by one
	}
	
	//set enable to low so busses are isolated, keep 2.7
	apuset(nOE, APU_FV, 0.4, APU_30V, APU_100MA, PIN_TO_VI );
	
	//wait
	lwait(1000);

	//measure leakage current from nOE
	apumi(nOE, 100, 10);
	//etsprintf("getting nOE results");
	groupgetresults(apu_leakL_NOE, 1);

	//log APU nOE results
	msLogResultAll(DSIndex++, apu_leakL_NOE, 1, 0, 1 );

	/*
	apumi( APU_PINS, 100, 10 );
	groupgetresults(apu_leakL, 4);

	for(i = 0; i < 2; i++) {
		msLogResultAll(DSIndex++, apu_leakL, 4, 1, 2); // log results one by one
	}
	*/

	//safe shutdown
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );
	
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: DigFunc
ETS_PRGFLOW_FUNC DigFunc( int DSIndex, LPCTSTR TestLabel )
{
	//trap switch
	if(!GetUserSwitch(GP3)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}

    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
    //Failure pins ptr
	INT64 FailPins_B2A, FailPins_A2B;
	int failures_B2A, failures_A2B;

	//set master clock, digital frequency, and digital timing/levels
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	//Turn power on
	sp100set(PWR, SP_FV, 5, SP_10V, SP_200MA);

	//enable device
	apuset(nOE, APU_FV, 0.0, APU_30V, APU_100MA, PIN_TO_VI);

// A TO B CONFIGURATION
	//set master clock, digital frequency, and digital timing/levels
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	apuset(DIR, APU_FV, 5.0, APU_30V, APU_100MA, PIN_TO_VI);
	dpinlevel( "digPins", "A2B", 5, 0, 2.4, 2, MS_SITE0 );
	dpintimeset("digPins", "A2B", "DTS", DPIN_TS_NORMAL, DPIN_NR, 200, 800, 500, 510, MS_SITE0);

	//load B pins to act like pull up resistor
	dpinviset( "B_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_512UA, 8, -2, MS_SITE0);
	lwait(1000);

	//run vector
	mcurun( MCU_NOANALOG, "A2B", 1 );

	//log results
	failures_A2B = dpingetfailpins("digPins", DPIN_ALL_PINS, &FailPins_A2B, MS_SITE0);
	msLogResult(MS_SITE0, DSIndex++, failures_A2B);
	dpinviset("B_PINS", DPIN_OFF, 0, DPIN_8V, DPIN_32MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_SITE0);
	

// B TO A CONFIGURATION
	
	//set master clock, digital frequency, and digital timing/levels
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	apuset(DIR, APU_FV, 0.0, APU_30V, APU_100MA, PIN_TO_VI);
	dpinlevel( "digPins", "B2A", 5, 0, 2.4, 2, MS_SITE0 );
	dpintimeset("digPins", "B2A", "DTS", DPIN_TS_NORMAL, DPIN_NR, 200, 800, 500, 510, MS_SITE0);

	//load A pins to act like pull up resistor
	dpinviset( "A_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_512UA, 8, -2, MS_SITE0);
    lwait(1000);

	//run vector
	mcurun( MCU_NOANALOG, "B2A", 1 );

	//log results
	failures_B2A = dpingetfailpins("digPins", DPIN_ALL_PINS, &FailPins_B2A, MS_SITE0);
	msLogResult(MS_SITE0, DSIndex++, failures_B2A);
	dpinviset("A_PINS", DPIN_OFF, 0, DPIN_8V, DPIN_32MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_SITE0);
	
//SHMOO PLOT AND NOE SWEEP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//set mcu and dpin period
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	//set levels and timing for B2A
	dpinlevel( "digPins", "B2A", 5, 0, 2.4, 2, MS_SITE0 );
	dpintimeset("digPins", "B2A", "DTS", DPIN_TS_NORMAL, DPIN_NR, 200, 800, 500, 510, MS_SITE0);

	//turn device on (device already on)
	sp100set(PWR, SP_FV, 5, SP_10V, SP_200MA);
	
	//reset DIR pin and set pullup load on A_PINS
	apuset(DIR, APU_FV, 0.0, APU_30V, APU_100MA, PIN_TO_VI);
	dpinviset( "A_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_512UA, 8, -2, MS_SITE0);

	//create for loop to increment nOE's voltage, run B2A, and report nOE's voltage only when failures are detected
	
	double i = 0;
	double failvoltage = 0;
	
	for (i = 0; i < 5; i+= 0.01) {
		failures_B2A = 0;
		apuset(nOE, APU_FV, i, APU_30V, APU_100MA, PIN_TO_VI);
        mcurun( MCU_NOANALOG, "B2A", 1 );
		failures_B2A = dpingetfailpins("digPins", DPIN_ALL_PINS, &FailPins_B2A, MS_SITE0);
		if (failures_B2A) {
			failvoltage = i;
			break;
		}
	}
	msLogResult(MS_SITE0, DSIndex++, failvoltage);

	
	//clean up pull up resistor
	dpinviset( "A_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_512UA, 8, -2, MS_SITE0);

	//repeat the previous loop for the reverse direction (decrement nOE from 5V to 0V)
	for (i = 5; i > 0; i-= 0.01) {
		failures_B2A = 0;
		apuset(nOE, APU_FV, i, APU_30V, APU_100MA, PIN_TO_VI);
        mcurun( MCU_NOANALOG, "B2A", 1 );
		failures_B2A = dpingetfailpins("digPins", DPIN_ALL_PINS, &FailPins_B2A, MS_SITE0);
		if (failures_B2A) {
			failvoltage = i;
			msLogResult(MS_SITE0, DSIndex++, failvoltage);
		}
	}
	
	//clean up aka shutdown code
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );

    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: Shmoo_TProp
ETS_PRGFLOW_FUNC Shmoo_TProp( int DSIndex, LPCTSTR TestLabel )
{
	//trap switch
	if(!GetUserSwitch(GP4)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}

    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );

	//Failure pins array
	INT64 FailPins_Shmoo;
	int failures_Shmoo;
     
	//set mcu and dpin period
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	//set levels and timing for B2A
	dpinlevel( "digPins", "delay_high", 5, 0, 2.4, 2, MS_SITE0 );
	dpintimeset("digPins", "delay_high", "DTS", DPIN_TS_NORMAL, DPIN_NR, 200, 800, 500, 510, MS_SITE0);

	//turn device on (device already on)
	sp100set(PWR, SP_FV, 5, SP_10V, SP_200MA);
	
	//reset DIR pin and set pullup load on A_PINS
	apuset(DIR, APU_FV, 0.0, APU_30V, APU_100MA, PIN_TO_VI);
	dpinviset( "A_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_512UA, 8, -2, MS_SITE0);
   
	//loop to capture data
	double i = 0;
	for (i = 2.4; i < 5; i+= 0.08) {
		failures_Shmoo = 0;
		dpinrcvset( "A_PINS", DPIN_RCV_IO, i, 2, MS_ALL );
        mcurun( MCU_NOANALOG, "delay_high", 1 );
		failures_Shmoo = dpingetfailpins("A_PINS", DPIN_ALL_PINS, &FailPins_Shmoo, MS_SITE0);
	}

	//shut down the MACHINE AAAAAAAAAAAA
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );

     
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: Dig_Sys_TProp
ETS_PRGFLOW_FUNC Dig_Sys_TProp( int DSIndex, LPCTSTR TestLabel )
{

	//trap switch
	if(!GetUserSwitch(GP4)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
     
	//Failure pins array
	INT64 FailPins;
	int failures;

    //initialize levels
	dpinlevel("digPins", "delay_high", 5, 0, 2.4, 2, MS_SITE0);

	//turn on device
	sp100set(PWR, SP_FV, 5, SP_10V, SP_200MA);

	//set to b2a and enable pins
	apuset(DIR, APU_FV, 0, APU_30V, APU_100MA, PIN_TO_VI);
	apuset(nOE, APU_FV, 0, APU_30V, APU_100MA, PIN_TO_VI);

	//load the abus
	dpinviset("A_PINS", DPIN_LOAD, 5, DPIN_10V, DPIN_32MA, 8, -2, MS_SITE0 );

	//set freq and period
	mcuset(MCU_MAIN, 50);
	dpinperiod(50);

	// construct loop to decrement EdgeC from 500ns to 200ns
	int i = 0;
	int failtime;
	for (i = 500; i > 200; i--) {
		failures = 0;
		dpintimeset("digPins", "delay_high", "DTS", DPIN_TS_NORMAL, DPIN_RZ, 200, DPIN_IGNORE_TIMING, i, DPIN_IGNORE_TIMING, MS_SITE0);
		mcurun(MCU_NOANALOG, "delay_high",1);
		failures = dpingetfailpins( "digPins", DPIN_ALL_PINS, &FailPins, MS_SITE0 );
		if (failures) {
			failtime = i;
			break;
		}
	}
	//once first failure occurs use corresponding EdgeC value to calc prop delay
	failtime -= 200;

	//log prop delay
	msLogResult(MS_SITE0, DSIndex++, failtime);
    
	//shut down the MACHINE AAAAAAAAAAAA
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );

    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC

// Test Function: Static_TProp
ETS_PRGFLOW_FUNC Static_TProp( int DSIndex, LPCTSTR TestLabel )
{
	//trap switch
	if(!GetUserSwitch(GP6)) {
		return (msSiteStat( MS_ALL) ); // if this hasn't been chosen, exit
	}
    //////////////////////////////////////
    // Update test numbering information
    SetTestNumber( TestNmbr[DSIndex] );
    SetSubTestNumber( SubTestNmbr[DSIndex] );
	
	/*
	//power device, set to b2a, and enable pins
	sp100set(VCC, SP_FV, 5, SP_10V, SP_20MA);
	apuset(DIR, APU_FV, 0, APU_30V, APU_100MA, PIN_TO_VI);
	apuset(nOE, APU_FV, 0, APU_30V, APU_100MA, PIN_TO_VI);

	//apply load to abus
	dpinviset(“ABUS”, DPIN_LOAD, 5, DPIN_10V, DPIN_32MA, 8, -2, MS_SITE0 );

	//set mcu to dut mode for free running clk
	//mcumode()
	//set receiver to all digital pins
	//set bus to tristate for listening
	//set bus to free running clock for the first 2 b pins
	//initialize and set qtmu mode to timer
	qtmumode( timeMeasure, QTMU_TIMER, QTMU_ARMOUTOFF );

	//start at b1, b2 receive high
	qtmustart( timeMeasure, "b1,b2", QTMU_RCVH, QTMU_NEG, 0, QTMU_NOFILTER, 0.0 );

	//stop at a1, a2 receive low
	qtmustop( timeMeasure, "a1,a2", QTMU_RCVL, QTMU_NEG, 0, QTMU_NOFILTER, 0.0 );

	//arm qtmu
	qtmuarm( timeMeasure, "AUTO", 1, QTMU_EDGE, QTMU_POS );

	//measure time for results
	double time = qtmumt(timeMeasure, QTMU_READTIME, 60000);
	msLogResult();

	//set tmu off

	//set bus to free running clk for next 2 b pins
	//start at b3, b4 receive high
	qtmustart( timeMeasure, "b3,b4", QTMU_RCVH, QTMU_NEG, 0, QTMU_NOFILTER, 0.0 );

	//stop at a3, a4 receive low
	qtmustop( timeMeasure, "a3,a4", QTMU_RCVL, QTMU_NEG, 0, QTMU_NOFILTER, 0.0 );

	//arm qtmu
	qtmuarm( timeMeasure, "AUTO", 1, QTMU_EDGE, QTMU_POS );

	//measure time for results and log the results
	double time = qtmumt(timeMeasure, QTMU_READTIME, 60000);
	msLogResult();

	//turn off the tmu
	//set bus to free running for the the next 2 pins
	*/

     
    //shut down the MACHINE AAAAAAAAAAAA
	apuset( nOE, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	apuset( DIR, APU_VIOFF, 0.0, APU_30V, APU_1MA, PIN_TO_VI );
	dpinviset( "digPins", DPIN_OFF, 0.0, DPIN_10V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	sp100set( PWR, SP_OFF, 0.0, SP_30V, SP_1A );

     
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
