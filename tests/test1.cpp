
#include "brig.h"
#include <stdio.h>

brig_MainWindow oMain;
brig_Edit oEdit;
brig_Label oLabel;
unsigned int idTimer = 0, idCount;

bool fncOnClose( brig_Widget *pDlg )
{
   SYMBOL_UNUSED( pDlg );
   return brig_MsgYesNo( (PBRIG_CHAR) "Really close window?", (PBRIG_CHAR) "Warning" );
}

void fncTimer( void )
{
   BRIG_CHAR szText[10];

   idCount++;
   sprintf( szText, "%d", idCount );
   oLabel.SetText( szText );
}

bool fncStart( brig_Widget *pBtn, WPARAM wParam, LPARAM lParam )
{
   SYMBOL_UNUSED( pBtn );
   SYMBOL_UNUSED( wParam );
   SYMBOL_UNUSED( lParam );

   if( !idTimer )
   {
      idCount = 0;
      idTimer = brigSetTimer( 1000, fncTimer );
   }
   oEdit.Show(0);
   return 0;
}

bool fncStop( brig_Widget *pBtn, WPARAM wParam, LPARAM lParam )
{
   SYMBOL_UNUSED( pBtn );
   SYMBOL_UNUSED( wParam );
   SYMBOL_UNUSED( lParam );

   if( idTimer )
   {
      brigKillTimer( idTimer );
      idTimer = 0;
   }
   oEdit.Show(1);
   return 0;
}

bool fncOnClick( brig_Widget *pBtn )
{
   PBRIG_CHAR pText;

   SYMBOL_UNUSED( pBtn );

   oEdit.SetTextColor( 255, 0 );
   oEdit.SetBackColor( 11184810, 1 );
   pText = oEdit.GetText();
   oLabel.SetText( pText );
   free( pText );

   return 0;
}

bool fncOnSize( brig_Widget *pPanel, WPARAM wParam, LPARAM lParam )
{
   unsigned long iWidth = ((unsigned long)lParam) & 0xFFFF;
   //unsigned long iHeight = ( ((unsigned long)lParam) >> 16 ) & 0xFFFF;

   SYMBOL_UNUSED( wParam );

   //brig_writelog( NULL, "fncOnSize: %d\r\n", iWidth );
   pPanel->Move( 0, 0, iWidth, 40 );
   return 0;

}

bool fncFocusIn1( brig_Widget *pEdit )
{
   SYMBOL_UNUSED( pEdit );

   oLabel.SetText( "In" );
   return 1;
}

bool fncFocusOu1( brig_Widget *pEdit )
{
   SYMBOL_UNUSED( pEdit );

   oLabel.SetText( "Out" );
   return 1;
}

void fncMenu1b( void )
{
   int iRes;
   char szRes[16];
   std::vector<char*> avList;

   avList.push_back( "Sergey Kropin" );
   avList.push_back( "Alex Haitek" );
   avList.push_back( "Mike Horn" );
   avList.push_back( "Iegudi Menuhin" );

   iRes = brigChoice( avList, "Select name", 400, 100, 200, 250 );
   sprintf( szRes, "Selected: %d", iRes );
   oLabel.SetText( szRes );
}

void fncMenu1e( void )
{
   oMain.Close();
}

bool fncCloseDlg( brig_Widget *pBtn, WPARAM wParam, LPARAM lParam )
{
   brig_Dialog *pDlg = ((brig_Dialog*)(pBtn->pParent));

   SYMBOL_UNUSED( wParam );
   SYMBOL_UNUSED( lParam );

   pDlg->pResult = (void*) pDlg->avWidgets[0]->GetText();
   pDlg->Close();
   return 1;
}

void fncMenu2( void )
{

   brig_Dialog oDlg;
   brig_Button oBtnD;
   brig_Edit oEdit1, oEdit2, oEdit3;

   oDlg.Create( &oMain, 200, 200, 200, 250, (PBRIG_CHAR) "A test dialog" );

   oEdit1.Create( &oDlg, 20, 20, 100, 28, NULL, WS_TABSTOP );
   oEdit2.Create( &oDlg, 20, 50, 100, 28, NULL, WS_TABSTOP );
   oEdit3.Create( &oDlg, 20, 80, 100, 28, NULL, WS_TABSTOP );

   oBtnD.Create( &oDlg, 50, 170, 100, 32, (PBRIG_CHAR) "Ok" );
   oBtnD.pfOnClick = fncCloseDlg;

   oDlg.Activate();
   if( oDlg.pResult )
   {
      brig_MsgInfo( (PBRIG_CHAR)oDlg.pResult, (PBRIG_CHAR) "Result" );
      free( oDlg.pResult );
   }
      
}

void fncMenu3( void )
{
   brig_MsgInfo( brig_Version(), (PBRIG_CHAR) "About" );
}


int brig_Main( int argc, char *argv[] )
{
   
   brig_Panel oPanel;
   brig_Button oBtn, oBtn1, oBtn2;
   brig_QButton oQBtn;
   brig_RadioGroup oRG;
   brig_RadioButton oR1, oR2;
   brig_CheckButton oCheck;
   brig_Combo oCombo;

   char *pCombo[3] = { "one", "two", "three" };
   long pColors1[2] = {0x333333, 0xcccccc};
   PBRIG_XMLITEM pXmlDoc;
   
   SYMBOL_UNUSED( argc );
   SYMBOL_UNUSED( argv );
   brig_writelog( NULL, "test %02d\r\n", 3 );
   /*
   for( int i = 0; i < argc; ++i ) {
      brig_writelog( NULL, "arg%d: %s\r\n", i, argv[i] );
   }
   */
   pXmlDoc = brigxml_GetDoc( "test1.xml" );
   if( !brigxml_Error() )
   {
      int iNum = 0;
      PBRIG_XMLITEM pParent, pNode;
      if( (pParent = brigxml_First( pXmlDoc )) != NULL )
         while( ( pNode = brigxml_Next( pParent, &iNum ) ) != NULL )
         {
            brig_writelog( NULL, "item: %s\r\n", pNode->szTitle );
         }
   }
   
   oMain.Create( 100, 100, 500, 320, (PBRIG_CHAR) "First Brig Window" );
   oMain.pfOnClose = fncOnClose;
   oMain.hFont = brigAddFont( "Georgia", 20 );
   
   brigMenu( &oMain );

   if( brigSubMenu( "File", 0 ) )
   {
      brigMenuItemAdd( "brigChoice", 0, fncMenu1b );
      brigMenuItemAdd( "Dialog", 0, fncMenu2 );
      brigMenuSeparator();
      brigMenuItemAdd( "Close", 0, fncMenu1e );
      brigMenuEnd();
   }

   brigMenuItemAdd( "Help", 0, fncMenu3 );

   brigMenuEnd();
   
   oPanel.Create( &oMain, 0, 0, 500, 40, brigAddStyle( 2, pColors1 ) );
   oPanel.pfOnSize = fncOnSize;

   oQBtn.Create( &oPanel, 0, 0, 48, 40, "Ok" );
   oQBtn.lBackColor = oQBtn.lBackClr1 = 0xcccccc;
   oQBtn.SetFont( brigAddFont( "Georgia", 18, 400, 0, 1 ) );

   oEdit.Create( &oMain, 20, 60, 100, 28 );
   oEdit.pfOnFocusIn  = fncFocusIn1;
   oEdit.pfOnFocusOut = fncFocusOu1;

   oCheck.Create( &oMain, 160, 60, 100, 28, "Mark me" );
   oLabel.Create( &oMain, 264, 60, 120, 24, "" );

   oRG.Begin( &oMain, 20, 100, 220, 90, "Radio group" );
   oR1.Create( &oRG, 30, 120, 150, 24, "radio1" );
   oR2.Create( &oRG, 30, 150, 150, 24, "radio2" );
   oRG.End( 1 );

   oBtn1.Create( &oMain, 264, 100, 76, 28, (PBRIG_CHAR) "Start" );
   oBtn1.pfOnClick = fncStart;

   oBtn2.Create( &oMain, 348, 100, 76, 28, (PBRIG_CHAR) "Stop" );
   oBtn2.pfOnClick = fncStop;

   oCombo.Create( &oMain, 264, 160, 120, 28, 4, 0, pCombo, 3 );

   oBtn.Create( &oMain, 100, 210, 100, 32, (PBRIG_CHAR) "Помощь" );
   oBtn.pfOnClick = fncOnClick;

   //brig_writelog( NULL, "edit id: %d\r\n", oEdit.iControlId );
   //brig_writelog( NULL, "button id: %d\r\n", oBtn.iControlId );

   oMain.Activate();

   if( pXmlDoc )
      brigxml_Release( pXmlDoc );

   return 0;
}

