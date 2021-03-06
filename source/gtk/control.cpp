/*
 * Brig - a ligthweight C++ GUI framework
 * GTK version
 * Standard Widgets creation and manipulation
 *
 * Copyright 2017 Alexander S.Kresin <alex@kresin.ru>
 * www - http://www.kresin.ru
 */

#include "brig.h"

extern void cb_signal( GtkWidget *widget, gchar* data );
extern void brig_SetEvent( gpointer handle, char * cSignal, long int p1, long int p2, long int p3 );
extern void brig_SetSignal( gpointer handle, char * cSignal, long int p1, long int p2, long int p3 );
extern void brig_parse_color( long lColor, GdkColor * pColor );

/* -------- Label ---------
 */
BRIG_HANDLE brig_CreateLabel( brig_Widget *pLabel, int iWidgId,
          int x, int y, int nWidth, int nHeight, unsigned long ulStyle,
          PBRIG_CHAR lpCaption, unsigned long ulExStyle )
{

   GtkWidget *hCtrl, *hLabel;
   GtkFixed *box;

   if( ( ulStyle & SS_OWNERDRAW ) == SS_OWNERDRAW )
      hCtrl = gtk_drawing_area_new();
   else
   {
      hCtrl = gtk_event_box_new(  );
      hLabel = gtk_label_new( lpCaption );
      gtk_container_add( GTK_CONTAINER( hCtrl ), hLabel );
      g_object_set_data( ( GObject * ) hCtrl, "label", ( gpointer ) hLabel );
      if( ulExStyle && WS_EX_TRANSPARENT )
      {
         gtk_event_box_set_visible_window( GTK_EVENT_BOX( hCtrl ), 0 );
      }

      if( !( ulStyle & SS_CENTER ) )
         gtk_misc_set_alignment( GTK_MISC( hLabel ),
               ( ulStyle & SS_RIGHT ) ? 1 : 0, 0 );
   }
   box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pLabel->pParent->Handle(), "fbox" );
   if( box )
      gtk_fixed_put( box, hCtrl, x, y );
   gtk_widget_set_size_request( hCtrl, nWidth, nHeight );

   if( ( ulStyle & SS_OWNERDRAW ) == SS_OWNERDRAW )
   {
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"expose_event", WM_PAINT, 0, 0 );
      gtk_widget_add_events( hCtrl, GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
            GDK_POINTER_MOTION_MASK );
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"button_press_event", 0, 0, 0 );
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"button_release_event", 0, 0, 0 );
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"motion_notify_event", 0, 0, 0 );
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"key_press_event", 0, 0, 0 );
      brig_SetEvent( ( gpointer ) hCtrl, (char*)"key_release_event", 0, 0, 0 );
   }
   return hCtrl;

}

/* -------- Button --------- */

BRIG_HANDLE brig_CreateButton( brig_Widget *pWidget, int iWidgId,
          int x, int y, int nWidth, int nHeight, unsigned long ulStyle, PBRIG_CHAR lpCaption )
{
   GtkWidget *hCtrl;
   GtkFixed *box;

   if( ( ulStyle & 0xf ) == BS_AUTORADIOBUTTON )
   {
      brig_RadioGroup *pGroup = ((brig_RadioButton*)pWidget)->pGroup;
      bool bFirst = ( pGroup->avButtons.size() == 0 );
      GSList *group = ( bFirst )? NULL :
         ( GSList * ) g_object_get_data( ( GObject * ) pGroup->avButtons[0]->Handle(), "list" );
      hCtrl = gtk_radio_button_new_with_label( group, lpCaption );
      group = gtk_radio_button_get_group( ( GtkRadioButton * ) hCtrl );
      if( bFirst )
         g_object_set_data( ( GObject * ) hCtrl, "list", ( gpointer ) group );
      else
         g_object_set_data( ( GObject * ) pGroup->avButtons[0]->Handle(), "list", ( gpointer ) group );
         
      //pWidget->pList = (void*) group;
   }
   else if( ( ulStyle & 0xf ) == BS_AUTOCHECKBOX )
      hCtrl = gtk_check_button_new_with_label( lpCaption );
   else if( ( ulStyle & 0xf ) == BS_GROUPBOX )
      hCtrl = gtk_frame_new( lpCaption );
   else
      hCtrl = gtk_button_new_with_mnemonic( lpCaption );

   box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pWidget->pParent->Handle(), "fbox" );
   if( box )
      gtk_fixed_put( box, hCtrl, x, y );
   gtk_widget_set_size_request( hCtrl, nWidth, nHeight );

   brig_SetSignal( hCtrl, (char*)"clicked", WM_LBUTTONUP, 0, 0 );

   return hCtrl;

}

bool brig_CheckBtnGet( brig_Widget *pWidget )
{
   return gtk_toggle_button_get_active( ( GtkToggleButton * ) pWidget->Handle() );

}

void brig_CheckBtnSet( brig_Widget *pWidget, bool bValue )
{
   gtk_toggle_button_set_active( ( GtkToggleButton * ) pWidget->Handle(), bValue );
}

bool brig_RadioBtnGet( brig_Widget *pWidget )
{
   return gtk_toggle_button_get_active( ( GtkToggleButton * ) pWidget->Handle() );
}

void brig_RadioBtnSet( brig_Widget *pWidget, bool bValue )
{
   gtk_toggle_button_set_active( ( GtkToggleButton * ) pWidget->Handle(), bValue );
}

void brig_RadioGroupSet( brig_RadioGroup *pGroup, int iSelected )
{
   gtk_toggle_button_set_active( ( GtkToggleButton * ) pGroup->avButtons[iSelected-1]->Handle(), 1 );
}


/* -------- Edit --------- 
 */

BRIG_HANDLE brig_CreateEdit( brig_Edit *pEdit, int iWidgId,
          int x, int y, int nWidth, int nHeight, unsigned long ulStyle,
          PBRIG_CHAR lpCaption, unsigned long ulExStyle )
{
   GtkWidget *hCtrl;

   if( ulStyle & ES_MULTILINE )
   {
      hCtrl = gtk_text_view_new(  );
      g_object_set_data( ( GObject * ) hCtrl, "multi", ( gpointer ) 1 );
      if( ulStyle & ES_READONLY )
         gtk_text_view_set_editable( ( GtkTextView * ) hCtrl, 0 );
      gtk_text_view_set_wrap_mode( GTK_TEXT_VIEW(hCtrl), GTK_WRAP_WORD_CHAR);
   }
   else
   {
      hCtrl = gtk_entry_new(  );
      if( ulStyle & ES_PASSWORD )
         gtk_entry_set_visibility( ( GtkEntry * ) hCtrl, FALSE );
   }

   GtkFixed *box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pEdit->pParent->Handle(), "fbox" );
   if( box )
      gtk_fixed_put( box, hCtrl, x, y );
   gtk_widget_set_size_request( hCtrl, nWidth, nHeight );

   if( lpCaption )
   {
      if( ulStyle & ES_MULTILINE )
      {
         GtkTextBuffer *buffer =
               gtk_text_view_get_buffer( GTK_TEXT_VIEW( hCtrl ) );
         gtk_text_buffer_set_text( buffer, lpCaption, -1 );
      }
      else
         gtk_entry_set_text( ( GtkEntry * ) hCtrl, lpCaption );
   }

   gtk_widget_add_events( hCtrl, GDK_BUTTON_PRESS_MASK );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"button_press_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"focus_in_event", WM_SETFOCUS, 0, 0 );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"focus_out_event", WM_KILLFOCUS, 0, 0 );

   //all_signal_connect( ( gpointer ) hCtrl );

   /*
   if( hEdit )
   {
      if( lpCaption )
      {
         PBRIG_WCHAR wcCaption = brig_str2WC( lpCaption );
         SendMessage( hEdit, WM_SETTEXT, 0, ( LPARAM ) wcCaption );
         brig_free( wcCaption );
      }
      SetWindowLongPtr( hEdit, GWLP_USERDATA, NULL );
      wpOrigEditProc = ( WNDPROC ) SetWindowLongPtr( hEdit,
            GWLP_WNDPROC, ( LONG_PTR ) s_EditProc );
   }
   */
   return hCtrl;

}

/* -------- Combobox --------- */

BRIG_HANDLE brig_CreateCombo( brig_Combo *pCombo, int iWidgId,
          int x, int y, int nWidth, int nHeight, int iDisplay, bool bEdit )
{
   GtkWidget *hCtrl;

   SYMBOL_UNUSED( iDisplay );

   if( bEdit )
      hCtrl = gtk_combo_box_entry_new_text();
   else
      hCtrl = gtk_combo_box_new_text();

   GtkFixed *box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pCombo->pParent->Handle(), "fbox" );
   if( box )
      gtk_fixed_put( box, hCtrl, x, y );
   gtk_widget_set_size_request( hCtrl, nWidth, nHeight );

   brig_SetSignal( hCtrl, (char*)"changed", CBN_SELCHANGE, 0, 0 );

   return hCtrl;
}

void brig_ComboSetArray( brig_Widget *pWidget, char **pArray, int iLen )
{
   for( int i = 0; i < iLen; i++ )
      gtk_combo_box_append_text( GTK_COMBO_BOX(pWidget->Handle()), pArray[i] );
}

int brig_ComboGetValue( brig_Widget *pWidget )
{
   return gtk_combo_box_get_active( (GtkComboBox*) (pWidget->Handle()) );
}

void brig_ComboSetValue( brig_Widget *pWidget, int iSelected )
{
   gtk_combo_box_set_active( (GtkComboBox*) (pWidget->Handle()), iSelected );
}

/* -------- Tab --------- */

void cb_signal_tab( GtkNotebook *notebook, GtkNotebookPage *page,
      guint page_num, gpointer user_data )
{
   gpointer gObject = g_object_get_data( (GObject*) notebook, "obj" );

   SYMBOL_UNUSED( page );
   SYMBOL_UNUSED( user_data );

   ( ( brig_Widget* ) gObject )->onEvent( WM_USER, 0, page_num );
}

BRIG_HANDLE brig_CreateTab( brig_Tab *pTab, int iWidgId,
          int x, int y, int nWidth, int nHeight )
{
   GtkWidget *hCtrl = gtk_notebook_new(  );
   GtkFixed *box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pTab->pParent->Handle(), "fbox" );

   if( box )
      gtk_fixed_put( box, hCtrl, x, y );
   gtk_widget_set_size_request( hCtrl, nWidth, nHeight );

   g_signal_connect( hCtrl, "switch-page",
                      G_CALLBACK (cb_signal_tab), NULL );

   return hCtrl;
}

void brig_TabAddPage( brig_Tab *pTab, int iPage, PBRIG_CHAR lpName )
{

   GtkNotebook *nb = ( GtkNotebook * ) pTab->Handle();
   GtkWidget *box = gtk_fixed_new();
   GtkWidget *hLabel = gtk_label_new( lpName );

   SYMBOL_UNUSED( iPage );

   gtk_notebook_append_page( nb, box, hLabel );

   g_object_set_data( ( GObject * ) nb, "fbox", ( gpointer ) box );

}

void brig_TabShowPage( brig_Tab *pTab, int iPage, bool bShow )
{
   SYMBOL_UNUSED( pTab );
   SYMBOL_UNUSED( iPage );
   SYMBOL_UNUSED( bShow );
}

/* -------- Panel --------- */

BRIG_HANDLE brig_CreatePanel( brig_Panel *pPanel, int iWidgId,
          int x, int y, int nWidth, int nHeight )
{
   BRIG_HANDLE hPanel;

   GtkWidget *vbox, *hbox;
   GtkWidget *vscroll = NULL, *hscroll = NULL;
   GtkFixed *box, *fbox;

   fbox = ( GtkFixed * ) gtk_fixed_new();

   hbox = gtk_hbox_new( FALSE, 0 );
   vbox = gtk_vbox_new( FALSE, 0 );

   //if( ( ulStyle & SS_OWNERDRAW ) == SS_OWNERDRAW )
      hPanel = gtk_drawing_area_new();
   //else
   //   hPanel = gtk_toolbar_new();

   gtk_box_pack_start( GTK_BOX( hbox ), vbox, TRUE, TRUE, 0 );
   /*if( ulStyle & WS_VSCROLL )
   {
      GtkObject *adjV;
      adjV = gtk_adjustment_new( 0.0, 0.0, 101.0, 1.0, 10.0, 10.0 );
      vscroll = gtk_vscrollbar_new( GTK_ADJUSTMENT( adjV ) );
      gtk_box_pack_end( GTK_BOX( hbox ), vscroll, FALSE, FALSE, 0 );

      //temp = HB_PUTHANDLE( NULL, adjV );
      //SetObjectVar( pObject, "_HSCROLLV", temp );
      //hb_itemRelease( temp );

      SetWindowObject( ( GtkWidget * ) adjV, pObject );
      set_signal( ( gpointer ) adjV, "value_changed", WM_VSCROLL, 0, 0 );
   }
   */
   gtk_box_pack_start( GTK_BOX( vbox ), (GtkWidget*)fbox, TRUE, TRUE, 0 );
   gtk_fixed_put( fbox, hPanel, 0, 0 );
   /*
   if( ulStyle & WS_HSCROLL )
   {
      GtkObject *adjH;
      adjH = gtk_adjustment_new( 0.0, 0.0, 101.0, 1.0, 10.0, 10.0 );
      hscroll = gtk_hscrollbar_new( GTK_ADJUSTMENT( adjH ) );
      gtk_box_pack_end( GTK_BOX( vbox ), hscroll, FALSE, FALSE, 0 );

      //temp = HB_PUTHANDLE( NULL, adjH );
      //SetObjectVar( pObject, "_HSCROLLH", temp );
      //hb_itemRelease( temp );

      SetWindowObject( ( GtkWidget * ) adjH, pObject );
      set_signal( ( gpointer ) adjH, "value_changed", WM_HSCROLL, 0, 0 );
   }*/

   box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pPanel->pParent->Handle(), "fbox" );
   if( box )
   {
      gtk_fixed_put( box, ( GtkWidget * ) hbox, x, y );
      g_object_set_data( ( GObject * ) hPanel, "pbox", ( gpointer ) hbox );
      gtk_widget_set_size_request( ( GtkWidget * ) hbox, nWidth, nHeight );
      if( vscroll )
         nWidth -= 12;
      if( hscroll )
         nHeight -= 12;
      gtk_widget_set_size_request( hPanel, nWidth, nHeight );
   }
   
   g_object_set_data( ( GObject * ) hPanel, "fbox", ( gpointer ) fbox );

   //temp = HB_PUTHANDLE( NULL, hbox );
   //SetObjectVar( pObject, "_HBOX", temp );
   //hb_itemRelease( temp );

   GTK_WIDGET_SET_FLAGS( hPanel, GTK_CAN_FOCUS );
   //if( ( ulStyle & SS_OWNERDRAW ) == SS_OWNERDRAW )
      brig_SetEvent( ( gpointer ) hPanel, (char*)"expose_event", WM_PAINT, 0, 0 );
   gtk_widget_add_events( hPanel, GDK_BUTTON_PRESS_MASK |
         GDK_BUTTON_RELEASE_MASK | GDK_ENTER_NOTIFY_MASK |
         GDK_LEAVE_NOTIFY_MASK );
   brig_SetEvent( ( gpointer ) hPanel, (char*)"button_press_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hPanel, (char*)"button_release_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hPanel, (char*)"enter_notify_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hPanel, (char*)"leave_notify_event", 0, 0, 0 );
   //all_signal_connect( ( gpointer ) hPanel );

   /*
   if( hPanel )
   {
      SetWindowLongPtr( hPanel, GWLP_USERDATA, NULL );
   }
   */
   return hPanel;
}

/* -------- QButton --------- 
 */

static gint cb_qbtn( GtkWidget *widget, GdkEvent * event, gchar* data )
{
   gpointer gObject = g_object_get_data( (GObject*) widget, "obj" );

   SYMBOL_UNUSED( data );

   if( gObject )
   {
      if( event->type == GDK_ENTER_NOTIFY || event->type == GDK_LEAVE_NOTIFY )
      {
         if( event->type == GDK_ENTER_NOTIFY )
            ((brig_QButton *)gObject)->iState = 1;
         else
            ((brig_QButton *)gObject)->iState = 0;
         brig_RedrawWindow( ((brig_QButton *)gObject) );
      }
   }
   return 0;
}

BRIG_HANDLE brig_CreateQButton( brig_QButton *pQBtn, int iWidgId,
          int x, int y, int nWidth, int nHeight )
{
   BRIG_HANDLE hQButton;

   GtkFixed *box;

   hQButton = gtk_drawing_area_new();

   box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pQBtn->pParent->Handle(), "fbox" );
   if( box )
   {
      gtk_fixed_put( box, hQButton, x, y );
      gtk_widget_set_size_request( hQButton, nWidth, nHeight );
   }
   brig_SetEvent( ( gpointer ) hQButton, (char*)"expose_event", WM_PAINT, 0, 0 );
   GTK_WIDGET_SET_FLAGS( hQButton, GTK_CAN_FOCUS );
   gtk_widget_add_events( hQButton, GDK_BUTTON_PRESS_MASK |
         GDK_BUTTON_RELEASE_MASK | GDK_ENTER_NOTIFY_MASK |
         GDK_LEAVE_NOTIFY_MASK );
   brig_SetEvent( ( gpointer ) hQButton, (char*)"button_press_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hQButton, (char*)"button_release_event", 0, 0, 0 );

   g_signal_connect( hQButton, (char*)"enter_notify_event",
         G_CALLBACK (cb_qbtn), NULL );
   g_signal_connect( hQButton, (char*)"leave_notify_event",
         G_CALLBACK (cb_qbtn), NULL );

   return hQButton;
}

/* -------- Splitter --------- 
 */

BRIG_HANDLE brig_CreateSplitter( brig_Splitter *pSplitter, int iWidgId,
          int x, int y, int nWidth, int nHeight )
{
   GtkWidget *hCtrl;
   GtkFixed *box;

   hCtrl = gtk_drawing_area_new(  );
   box = ( GtkFixed * ) g_object_get_data( ( GObject * ) pSplitter->pParent->Handle(), "fbox" );

   if( box )
   {
      gtk_fixed_put( box, ( GtkWidget * ) hCtrl, x, y );
      gtk_widget_set_size_request( ( GtkWidget * ) hCtrl, nWidth, nHeight );
   }

   brig_SetEvent( ( gpointer ) hCtrl, (char*)"expose_event", WM_PAINT, 0, 0 );
   GTK_WIDGET_SET_FLAGS( hCtrl, GTK_CAN_FOCUS );

   gtk_widget_add_events( hCtrl, GDK_BUTTON_PRESS_MASK |
         GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"button_press_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"button_release_event", 0, 0, 0 );
   brig_SetEvent( ( gpointer ) hCtrl, (char*)"motion_notify_event", 0, 0, 0 );

   return hCtrl;
}

/* -------- common widget's functions --------- */

PBRIG_CURSOR brig_LoadCursor( int iCursorType )
{
   return gdk_cursor_new( ( GdkCursorType ) iCursorType );
}

void brig_SetCursor( PBRIG_CURSOR hCursor, brig_Widget *pWidget )
{
   GtkWidget *widget;

   if( !pWidget )
      pWidget = brig_GetActiveWindow();
   widget = ( pWidget ) ? pWidget->Handle() : NULL;
   gdk_window_set_cursor( widget->window, hCursor );
}

void brig_SetFgColor( brig_Widget *pWidget, long lColor )
{

   GtkWidget *label;
   BRIG_HANDLE hCtrl = pWidget->Handle();

   if( GTK_IS_BUTTON( hCtrl ) )
   {
      label = gtk_bin_get_child( GTK_BIN( hCtrl ) );
   }
   else if( GTK_IS_EVENT_BOX( hCtrl ) )
      label = gtk_bin_get_child( GTK_BIN( hCtrl ) );
   else
   {
      label = hCtrl; //g_object_get_data( ( GObject * ) hCtrl, "label" );
   }

   if( label )
   {
      GdkColor color;
      
      brig_parse_color( lColor, &color );
      gtk_widget_modify_fg( hCtrl, GTK_STATE_NORMAL, &color );
      gtk_widget_modify_text( hCtrl, GTK_STATE_NORMAL, &color );

      /* GtkStyle * style = gtk_style_copy( gtk_widget_get_style( label ) );
      brig_parse_color( lColor, &(style->fg[GTK_STATE_NORMAL]) );
      brig_parse_color( lColor, &(style->text[GTK_STATE_NORMAL]) );
      gtk_widget_set_style( label, style ); */
   }
}

void brig_SetBgColor( brig_Widget *pWidget, long lColor )
{
   GdkColor color;
   BRIG_HANDLE hCtrl = pWidget->Handle();
   
   brig_parse_color( lColor, &color );
   gtk_widget_modify_bg( hCtrl, GTK_STATE_NORMAL, &color );
   gtk_widget_modify_base( hCtrl, GTK_STATE_NORMAL, &color );

   /* GtkStyle * style = gtk_style_copy( gtk_widget_get_style( hCtrl ) );
   brig_parse_color( lColor, &(style->bg[GTK_STATE_NORMAL]) );
   brig_parse_color( lColor, &(style->base[GTK_STATE_NORMAL]) );
   gtk_widget_set_style( hCtrl, style ); */

}

static gint cb_timer( gchar * data )
{
   unsigned int uiId, idTimer = 0;

   sscanf( ( char * ) data, "%u", &uiId );

   for( unsigned int i = 0; i < brigApp.avTimers.size(); i++ )
      if( brigApp.avTimers[i].uiIdInit == uiId )
      {
         idTimer = brigApp.avTimers[i].uiId;
         break;
      }

   brigRunTimerFunc( idTimer );
   return 1;
}

unsigned int brig_SetTimer( unsigned int uiId, unsigned int uiValue )
{

   char buf[10] = { 0 };
   sprintf( buf, "%u", uiId );
   return ( gint ) gtk_timeout_add( ( guint32 ) uiValue,
               ( GtkFunction ) cb_timer, g_strdup( buf ) );

}

void brig_KillTimer( unsigned int uiId )
{

   gtk_timeout_remove( ( gint ) uiId );
}

int brig_GetScrollPos( brig_Widget *pWidget, bool bVertical )
{
   GtkAdjustment *adj = (GtkAdjustment *) g_object_get_data(
         ( GObject * ) pWidget->Handle(), (bVertical)? "adjv" : "adjh" );
   if( adj )
      return adj->value;

   return -1;
}

void brig_SetScrollPos( brig_Widget *pWidget, bool bVertical, int iPos )
{
   GtkAdjustment *adj = (GtkAdjustment *) g_object_get_data(
         ( GObject * ) pWidget->Handle(), (bVertical)? "adjv" : "adjh" );
   if( adj )
   {
      if( iPos > (adj->upper - adj->page_size) )
         iPos = adj->upper - adj->page_size;
      adj->value = iPos;
      gtk_adjustment_changed( adj );
      g_object_set_data( ( GObject * ) pWidget->Handle(),
            (bVertical)? "iscrv" : "iscrh", ( gpointer ) iPos );
   }

}

int brig_GetScrollRange( brig_Widget *pWidget, bool bVertical, int *pMinPos, int *pMaxPos )
{
   GtkAdjustment *adj = (GtkAdjustment *) g_object_get_data(
         ( GObject * ) pWidget->Handle(), (bVertical)? "adjv" : "adjh" );
   if( adj )
   {
      if( pMinPos )
         *pMinPos = (int) adj->lower;
      if( pMaxPos )
         *pMaxPos = (int) (adj->upper - adj->page_size);
      return (int) (adj->upper - -adj->page_size - adj->lower);
   }

   return -1;
}

void brig_SetScrollRange( brig_Widget *pWidget, bool bVertical, int iMinPos, int iMaxPos )
{
   GtkAdjustment *adj = (GtkAdjustment *) g_object_get_data(
         ( GObject * ) pWidget->Handle(), (bVertical)? "adjv" : "adjh" );
   if( adj )
   {
      adj->lower = (gfloat) iMinPos;
      adj->upper = (gfloat) (iMaxPos + adj->page_size);
      gtk_adjustment_changed( adj );
   }
}

bool brig_AddToolTip( brig_Widget *pWidget, PBRIG_CHAR lpText )
{

   gtk_widget_set_tooltip_text( ( GtkWidget * ) pWidget->Handle(), lpText );
   return 1;

}

void brig_DelToolTip( brig_Widget *pWidget )
{

   gtk_widget_set_tooltip_text( ( GtkWidget * ) pWidget->Handle(), NULL );

}

void brig_SetToolTipText( brig_Widget *pWidget, PBRIG_CHAR lpText )
{

   gtk_widget_set_tooltip_text( ( GtkWidget * ) pWidget->Handle(), lpText );

}
