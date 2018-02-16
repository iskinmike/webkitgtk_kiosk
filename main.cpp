#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <gdk/gdkkeysyms.h>
#include <X11/Xlib.h>

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);

const int minimize_offset = 300;

bool fullscreen_state = true;

Display* d = XOpenDisplay(NULL);
Screen*  s = DefaultScreenOfDisplay(d);

void swichFullscreenMode(GtkWidget *widget){
    GdkGeometry geom;
    if (fullscreen_state) {
        printf("Set sized\n");
        gtk_window_unfullscreen(GTK_WINDOW(widget));
        gtk_window_unmaximize(GTK_WINDOW(widget));
        // set window size
        geom.max_width = s->width - minimize_offset;
        geom.max_height = s->height - minimize_offset;
    } else {
        printf("Set fullscreen\n");
        gtk_window_maximize(GTK_WINDOW(widget));
        gtk_window_fullscreen(GTK_WINDOW(widget));

        geom.max_width = s->width;
        geom.max_height = s->height;// + minimize_offset;
    }

    gtk_window_set_default_size(GTK_WINDOW(widget), geom.max_width, geom.max_height);
    gtk_window_set_geometry_hints(GTK_WINDOW(widget), NULL, &geom, GDK_HINT_MAX_SIZE);

    fullscreen_state = !fullscreen_state;
    printf("fullscreeen state: '%s'\n", (fullscreen_state) ? "true" : "false" );
    printf("screen size: %dx%d \n", geom.max_width, geom.max_height);
}

gboolean console_message_handler(WebKitWebView *web_view, gchar *message,
        gint line, gchar *source_id, gpointer user_data)
{
    printf("[CONSOLE LOG]: '%s', [Line '%d'], [Source '%s'], [Pointer: %x]", message, line, source_id, web_view);
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  // suppress warnings
  (void) widget;
  (void) user_data;
  // handle keys
  switch (event->keyval)
  {
    case GDK_KEY_p:
      printf("key release: %s\n", "p");
      swichFullscreenMode(widget);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

int main(int argc, char* argv[])
{
  // Initialize GTK+
  gtk_init(&argc, &argv);

  // Create window that will contain the browser instance
  GtkWidget* main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // Define max geometry of window

  gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);

  // Create a browser instance
  WebKitWebView* webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  gtk_window_fullscreen(GTK_WINDOW(main_window));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "key_release_event", G_CALLBACK(on_key_release), NULL);
  g_signal_connect(main_window, "console-message", G_CALLBACK(console_message_handler), NULL);
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // Load a web page into the browser instance
  webkit_web_view_load_uri(webView, "http://localhost:8080/");

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Run the main GTK+ event loop
  gtk_main();

  return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
  // suppress warnings
  (void) widget;
  (void) window;
  gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
  // suppress warnings
  (void) webView;
  gtk_widget_destroy(window);
  return TRUE;
}
