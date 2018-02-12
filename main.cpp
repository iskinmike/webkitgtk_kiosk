

//#include <QCoreApplication>

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <gdk/gdkkeysyms.h>

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);


GtkWidget *main_window;
GtkWidget *view_widget;
WebKitWebView *webView;
bool fullscreen_state = true;

void swichFullscreenMode(){

//    printf("Is toplevel   '%d'\n", gtk_widget_is_toplevel(view_widget));
//    printf("Is sensitive  '%d'\n", gtk_widget_is_sensitive(view_widget));
//    printf("Is focus      '%d'\n", gtk_widget_is_focus(view_widget));
//    printf("Is composited '%d'\n", gtk_widget_is_composited(view_widget));
//    printf("Is visible    '%d'\n", gtk_widget_is_visible(view_widget));
//    printf("Is resizble   '%d'\n", gtk_window_get_resizable(GTK_WINDOW(main_window)));

//    webView

//     WebKitWindowProperties* props = webkit_web_view_get_window_properties(webView);
//     props
//     WebKitWebWindowFeatures* features = webkit_web_view_get_window_features(webView);
//     features->;
//     WebKitWebWindowFeatures test;



    if (fullscreen_state) {
        printf("Set sized\n");
        gtk_window_unfullscreen(GTK_WINDOW(main_window));
//        gtk_window_unmaximize(GTK_WINDOW(main_window));
//        gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
//        gtk_widget_set_size_request(GTK_WIDGET(webView), 640, 480);
    } else {
        printf("Set fullscreen\n");
//        gtk_window_maximize(GTK_WINDOW(main_window));
        gtk_window_fullscreen(GTK_WINDOW(main_window));
        // Make sure the main window and all its contents are visible
//        gtk_widget_show_all(main_window);
    }
    fullscreen_state = !fullscreen_state;
    printf("fullscreeen state: '%s'\n", (fullscreen_state) ? "true" : "false" );
}


gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  switch (event->keyval)
  {
    case GDK_KEY_p:
      printf("key release: %s\n", "p");
      swichFullscreenMode();
      break;
    default:
      return FALSE;
  }

  return TRUE;
}


gboolean on_window_state_event (GtkWidget *widget, GdkEvent *event, gpointer user_data){

//    event-
    printf("State changes: %s\n", "*************************");
    return FALSE;
}

int main(int argc, char* argv[])
{
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create an 800x600 window that will contain the browser instance
  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);

  GdkGeometry geom;
  geom.min_width = 600;
  geom.min_height = 400;
  geom.max_width = 1200;
  geom.max_height = 1200;

//  GdkWindowHints  GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE

  gtk_window_set_geometry_hints(GTK_WINDOW(main_window), NULL, &geom, GDK_HINT_MAX_SIZE);
  gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);

//  gtk_window_set_
  view_widget = webkit_web_view_new();

  // Create a browser instance
  webView = WEBKIT_WEB_VIEW(view_widget);

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  gtk_window_fullscreen(GTK_WINDOW(main_window));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit

  g_signal_connect(main_window, "window-state-event", G_CALLBACK(on_window_state_event), NULL);
  g_signal_connect(main_window, "key_release_event", G_CALLBACK(on_key_press), NULL);
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(webView, "http://www.webkitgtk.org/");

  WebKitWindowProperties* window_properties = webkit_web_view_get_window_properties(webView);
//  g_signal_connect (window_properties, "notify::geometry",
//                    G_CALLBACK (window_geometry_changed), window);
//  g_signal_connect (window_properties, "notify::toolbar-visible",
//                    G_CALLBACK (window_toolbar_visibility_changed), window);
//  g_signal_connect (window_properties, "notify::menubar-visible",
//                    G_CALLBACK (window_menubar_visibility_changed), window);

  /* Apply the window properties before showing the window */
//  visible = webkit_window_properties_get_toolbar_visible (window_properties);
//  browser_window_set_toolbar_visible (BROWSER_WINDOW (window), visible);
//  visible = webkit_window_properties_get_menubar_visible (window_properties);
//  browser_window_set_menubar_visible (BROWSER_WINDOW (window), visible);
  printf("Is toolbar_visible '%d'\n", webkit_window_properties_get_toolbar_visible(window_properties));
  printf("Is menubar_visible    '%d'\n", webkit_window_properties_get_menubar_visible(window_properties));
  GdkRectangle rect;
  webkit_window_properties_get_geometry(window_properties, &rect);

  printf("max_width:  '%d'\n", rect.width  );
  printf("max_height: '%d'\n", rect.height );

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

//  gtk_widget_set_size_request(GTK_WIDGET(webView), 640, 480);

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Run the main GTK+ event loop
  gtk_main();

  return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
  gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
  gtk_widget_destroy(window);
  return TRUE;
}
