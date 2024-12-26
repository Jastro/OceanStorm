float text_scroll_position = ScreenHeight;
float text_position_for_show_message = 150;
bool can_close_ending = false;

int[128] english_text_1 = "With the prototype destroyed and \n"
                          "defenses eliminated, the Black Hand's \n"
                          "threat has been neutralized. \n";
int[128] english_text_2 = "The mission was successful, but \n"
                          "the war continues. Although heroes \n"
                          "like Alex Rykov bring hope, \n";
int[128] english_text_3 = "the world remains on the brink. \n"
                          "For now, Thalassa Island is free... \n"
                          "but for how long?";

int[128] spanish_text_1 = "Con el prototipo destruido y las \n"
                          "defensas eliminadas, la amenaza de \n"
                          "La Mano Negra ha sido neutralizada. \n";
int[128] spanish_text_2 = "La misi" o_ "n ha sido un " e_ "xito, pero \n"
                          "la guerra contin" u_ "a. Aunque h" e_ "roes \n"
                          "como " A_ "lex Rykov traen esperanza, \n";
int[128] spanish_text_3 = "el mundo sigue al borde del abismo. \n"
                          "Por ahora, la isla Thalassa es libre...\n" interr "por cu" a_ "nto tiempo?";
void update_ending()
{
    text_scroll_position -= 0.5;

    if (can_close_ending && gamepad_button_start() == 1)
    {
        // Restaurar color
        set_multiply_color(color_white);
        
        // Reiniciar todo el juego
        initialize_events();
        reset_game();
        begin_menu();
        
        play_sound(SoundMenuAccept);
    }
}

void render_ending()
{
    // Fondo
    set_multiply_color(0x80FFFFFF);
    select_texture(TextureEnding);
    select_region(0);
    draw_region_at(0, 0);

    // Texto scrolleando
    //set_multiply_color(color_yellow);
    set_multiply_color(color_white);
    float pos = text_scroll_position;
    if (game_language == 0)
    {
        
        print_at(ScreenCenterX - 200, pos, english_text_1);
        print_at(ScreenCenterX - 200, pos + 80, english_text_2);
        print_at(ScreenCenterX - 200, pos + 160, english_text_3);

        // Texto parpadeante al final
        if (pos < -text_position_for_show_message && (get_frame_counter() / BlinkRate) % 2 == 0)
        {
            set_multiply_color(color_yellow);
            can_close_ending = true;
            print_at(ScreenCenterX - 145, ScreenHeight - 40,
                     "Press START to return to menu");
            set_multiply_color(color_white);
        }
    }
    else
    {
        print_at(ScreenCenterX - 200, pos, spanish_text_1);
        print_at(ScreenCenterX - 200, pos + 80, spanish_text_2);
        print_at(ScreenCenterX - 200, pos + 160, spanish_text_3);

        if (pos < -text_position_for_show_message && (get_frame_counter() / BlinkRate) % 2 == 0)
        {
            set_multiply_color(color_yellow);
            can_close_ending = true;
            print_at(ScreenCenterX - 155, ScreenHeight - 40,
                     "Pulsa START para volver al men" u_ "");
            set_multiply_color(color_white);
        }
    }
}