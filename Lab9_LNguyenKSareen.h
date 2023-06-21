/*****************************************************************************
*                                                                            *
*       Source title:   Lab9_LNguyenKSareen.H
*                       main header file for the application                 *
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   mm/dd/yy                                             *
*        Current Rev:   r.rr                                                 *
*                                                                            *
*        Description:                                                        *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// CLab9_LNguyenKSareen:
// See Lab9_LNguyenKSareen.cpp for the implementation of this class
//
//{{AFX_DEFINE_ACE_GROUP( Datasheet Array Size )
#define DSSIZE 101
//}}AFX_DEFINE_ACE_GROUP
//{{AFX_ETS_PIN_DEFINITION
// Caution::  The following pin definitions are filled in by the PinMap Editor.
#define ADD_PIN( pinname, notes, resource )  \
	PINMAP_T( #pinname, notes, pinname, resource, \
		pinname##_S0 ),

#define      PIN_DEFINITION( pinname, pingrpval, site0 ) \
	int const pinname##_S0 = site0; \
	int const pinname = pingrpval;

	PIN_DEFINITION (TMU4, 8198 , 3)	// defines  TMU4_S0
	PIN_DEFINITION (TMU3, 8197 , 2)	// defines  TMU3_S0
	PIN_DEFINITION (TMU2, 8196 , 1)	// defines  TMU2_S0
	PIN_DEFINITION (TMU1, 8195 , 0)	// defines  TMU1_S0
	PIN_DEFINITION (PWR, 8194 , 32)	// defines  PWR_S0
	PIN_DEFINITION (nOE, 8193 , 9)	// defines  nOE_S0
	PIN_DEFINITION (DIR, 8192 , 8)	// defines  DIR_S0
//}}AFX_ETS_PIN_DEFINITION

//{{AFX_ETS_GROUP_DEFINITION
// Caution:: The following group definitions are filled in by the PinMap Editor
	GROUP_DEFINITION (TMUPINS, 16386)
	GROUP_DEFINITION (TMU02, 16387)
	GROUP_DEFINITION (TMU01, 16385)
	GROUP_DEFINITION (APU_PINS, 16384)
//}}AFX_ETS_GROUP_DEFINITION


















//{{AFX_ETS_FUNCTION_PROTOTYPES
ETS_PRGFLOW_FUNC Test( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Leak_Max( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Leak_High( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Leak_Low( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC DigFunc( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Shmoo_TProp( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Dig_Sys_TProp( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Static_TProp( int DSIndex, LPCTSTR TestLabel );
//}}AFX_ETS_FUNCTION_PROTOTYPES

class CLab9_LNguyenKSareen : public CTestMain
{
public:
    CLab9_LNguyenKSareen(PRG_FLOW *UserFlow) : CTestMain(UserFlow) {};
    ~CLab9_LNguyenKSareen(){};

//////////////////////////////////////////////////////////////////////////////
// Overridable Functions
//
//  To eliminate the need for any of these functions,
//  Remove their declaration below, as well as the corresponding
//  Function body in the Lab9_LNguyenKSareen.cpp file
//
    virtual BOOL  UserInit();                   // Called when user starts Application Program
    virtual BOOL  UserExit();                   // Called when user quits Application Program
    virtual BOOL  OnTestStartup();              // Called Before Program Flow Structure Execution
    virtual BOOL  OnTestCompletion();           // Called After Program Flow Structure Execution
    virtual BOOL  OnTestInit();                 // Called before 'GO / QUIT' buttons Displayed
    virtual BOOL  OnTestExit();                 // Called after 'QUIT' button pressed
    virtual BOOL  OnLogData();                  // Called during Data logging
    virtual BOOL  OnFailSite(int site);         // Called during the msFailSite() utility
    virtual int   OnSot(int SotValue);          // Called while waiting for Start Of Test signal
    virtual BOOL  OnUtilError(int errnum);      // Called by the Utilities when an ETS Util Error occurs
    virtual BOOL  OnInterlock(ULONG intlocks);  // Called during the interlock check...
};

//////////////////////////////////////////////////////////////////////////////
//
// ACE Define Block
//
// ETS AceWizard generated define blocks
//
//{{AFX_DEFINE_ACE_GROUP( Miscellaneous Defines )
    #define FOR_EACH_SITE(s,m) \
                for ( (s) = 0; (s) < (m); (s)++ ) \
                    if ( msSiteStat( (s) ) )
//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 1 )
    // NOTE: Add define members here for the group "Group Block 1"
#define NUM_SITES 1
//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 2 )
    // NOTE: Add define members here for the group "Group Block 2"
//}}AFX_DEFINE_ACE_GROUP
