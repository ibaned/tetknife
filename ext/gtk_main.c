#include <gtk/gtk.h>
#include "../front.h"

static GtkWidget* image;
static GdkPixbuf* pixbuf;

static void close_window(GtkWidget* widget, gpointer data)
{
  gtk_main_quit();
}

static void render(void)
{
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

static gboolean mouse_down(GtkWidget* w, GdkEventButton* event, gpointer u)
{
  front_mouse_down(event->x, event->y);
  return FALSE;
}

static gboolean mouse_up(GtkWidget* w, GdkEventButton* event, gpointer u)
{
  front_mouse_up(event->x, event->y);
  render();
  return FALSE;
}

static gboolean key_down(GtkWidget* w, GdkEventKey* event, gpointer u)
{
  front_key_down(event->string[0]);
  return FALSE;
}

static gboolean key_up(GtkWidget* w, GdkEventKey* event, gpointer u)
{
  front_key_up();
  render();
  return FALSE;
}

int main(int argc, char** argv)
{
  GtkWidget* window;
  GtkWidget* event_box;
  int event_mask;
  int width;
  int height;
  front_start();
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);
  width = front_width();
  height = front_height();
  pixbuf = gdk_pixbuf_new_from_data(front_pixels(), GDK_COLORSPACE_RGB,
      FALSE, 8, width, height, width * 3, NULL, NULL);
  image = gtk_image_new_from_pixbuf(pixbuf);
  event_box = gtk_event_box_new();
  event_mask = GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK;
  gtk_widget_add_events(event_box, event_mask);
  g_signal_connect(event_box, "button-press-event",
      G_CALLBACK(mouse_down), 0);
  g_signal_connect(event_box, "button-release-event",
      G_CALLBACK(mouse_up), 0);
  gtk_container_add(GTK_CONTAINER(event_box), image);
  g_signal_connect(window, "key-press-event", G_CALLBACK(key_down), 0);
  g_signal_connect(window, "key-release-event", G_CALLBACK(key_up), 0);
  gtk_container_add(GTK_CONTAINER(window), event_box);
  gtk_widget_show_all(window);
  gtk_main();
  front_stop();
  return 0;
}
