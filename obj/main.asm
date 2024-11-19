; program start section
  call __global_scope_initialization
  call __function_main
  hlt

; location of global variables
  %define global_airplane_x 0
  %define global_airplane_y 1
  %define global_camera_x 2
  %define global_camera_y 3
  %define global_airplane_angle 4
  %define global_airplane_scale 5
  %define global_airplane_velocity 6
  %define global_fuel 7
  %define global_airplane_frame 8
  %define global_anim_timer 9
  %define global_malloc_start_address 10
  %define global_malloc_end_address 11
  %define global_malloc_first_block 12
  %define global_island_x 13
  %define global_island_y 23
  %define global_island_tiles 33
  %define global_num_islands 153
  %define global_game_state 154

__global_scope_initialization:
  push BP
  mov BP, SP
  mov R0, 1048576
  mov [global_malloc_start_address], R0
  mov R0, 3145727
  mov [global_malloc_end_address], R0
  mov R0, -1
  mov [global_malloc_first_block], R0
  mov SP, BP
  pop BP
  ret

__function_make_gray:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  shl R0, 16
  or R0, -16777216
  mov R1, [BP+2]
  shl R1, 8
  or R0, R1
  mov R1, [BP+2]
  or R0, R1
__function_make_gray_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_make_color_rgb:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+4]
  shl R0, 16
  or R0, -16777216
  mov R1, [BP+3]
  shl R1, 8
  or R0, R1
  mov R1, [BP+2]
  or R0, R1
__function_make_color_rgb_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_make_color_rgba:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+5]
  shl R0, 24
  mov R1, [BP+4]
  shl R1, 16
  or R0, R1
  mov R1, [BP+3]
  shl R1, 8
  or R0, R1
  mov R1, [BP+2]
  or R0, R1
__function_make_color_rgba_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_get_color_red:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  and R0, 255
__function_get_color_red_return:
  mov SP, BP
  pop BP
  ret

__function_get_color_green:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  shl R0, -8
  and R0, 255
__function_get_color_green_return:
  mov SP, BP
  pop BP
  ret

__function_get_color_blue:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  shl R0, -16
  and R0, 255
__function_get_color_blue_return:
  mov SP, BP
  pop BP
  ret

__function_get_color_alpha:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  shl R0, -24
  and R0, 255
__function_get_color_alpha_return:
  mov SP, BP
  pop BP
  ret

__function_select_texture:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_SelectedTexture, R0
__function_select_texture_return:
  mov SP, BP
  pop BP
  ret

__function_get_selected_texture:
  push BP
  mov BP, SP
  in R0, GPU_SelectedTexture
__function_get_selected_texture_return:
  mov SP, BP
  pop BP
  ret

__function_select_region:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_SelectedRegion, R0
__function_select_region_return:
  mov SP, BP
  pop BP
  ret

__function_get_selected_region:
  push BP
  mov BP, SP
  in R0, GPU_SelectedRegion
__function_get_selected_region_return:
  mov SP, BP
  pop BP
  ret

__function_define_region:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_RegionMinX, R0
  mov R0, [BP+3]
  out GPU_RegionMinY, R0
  mov R0, [BP+4]
  out GPU_RegionMaxX, R0
  mov R0, [BP+5]
  out GPU_RegionMaxY, R0
  mov R0, [BP+6]
  out GPU_RegionHotSpotX, R0
  mov R0, [BP+7]
  out GPU_RegionHotSpotY, R0
__function_define_region_return:
  mov SP, BP
  pop BP
  ret

__function_define_region_topleft:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_RegionMinX, R0
  out GPU_RegionHotSpotX, R0
  mov R0, [BP+3]
  out GPU_RegionMinY, R0
  out GPU_RegionHotSpotY, R0
  mov R0, [BP+4]
  out GPU_RegionMaxX, R0
  mov R0, [BP+5]
  out GPU_RegionMaxY, R0
__function_define_region_topleft_return:
  mov SP, BP
  pop BP
  ret

__function_define_region_center:
  push BP
  mov BP, SP
  isub SP, 2
  mov R0, [BP+2]
  mov R1, [BP+4]
  iadd R0, R1
  idiv R0, 2
  mov [BP-1], R0
  mov R0, [BP+3]
  mov R1, [BP+5]
  iadd R0, R1
  idiv R0, 2
  mov [BP-2], R0
  mov R0, [BP+2]
  out GPU_RegionMinX, R0
  mov R0, [BP+3]
  out GPU_RegionMinY, R0
  mov R0, [BP+4]
  out GPU_RegionMaxX, R0
  mov R0, [BP+5]
  out GPU_RegionMaxY, R0
  mov R0, [BP-1]
  out GPU_RegionHotSpotX, R0
  mov R0, [BP-2]
  out GPU_RegionHotSpotY, R0
__function_define_region_center_return:
  mov SP, BP
  pop BP
  ret

__function_define_region_matrix:
  push BP
  mov BP, SP
  isub SP, 17
  mov R0, [BP+2]
  mov [BP-1], R0
  mov R0, [BP+3]
  mov [BP-2], R0
  mov R0, [BP+4]
  mov [BP-3], R0
  mov R0, [BP+5]
  mov [BP-4], R0
  mov R0, [BP+6]
  mov [BP-5], R0
  mov R0, [BP+7]
  mov [BP-6], R0
  mov R0, [BP+8]
  mov [BP-7], R0
  mov R0, [BP-4]
  mov R1, [BP-2]
  isub R0, R1
  iadd R0, 1
  mov R1, [BP+11]
  iadd R0, R1
  mov [BP-8], R0
  mov R0, [BP-5]
  mov R1, [BP-3]
  isub R0, R1
  iadd R0, 1
  mov R1, [BP+11]
  iadd R0, R1
  mov [BP-9], R0
  mov R0, 0
  mov [BP-10], R0
__for_206_start:
  mov R0, [BP-10]
  mov R1, [BP+10]
  ilt R0, R1
  jf R0, __for_206_end
  mov R0, 0
  mov [BP-11], R0
__for_216_start:
  mov R0, [BP-11]
  mov R1, [BP+9]
  ilt R0, R1
  jf R0, __for_216_end
  mov R1, [BP-1]
  mov [SP], R1
  call __function_select_region
  mov R1, [BP-2]
  mov [SP], R1
  mov R1, [BP-3]
  mov [SP+1], R1
  mov R1, [BP-4]
  mov [SP+2], R1
  mov R1, [BP-5]
  mov [SP+3], R1
  mov R1, [BP-6]
  mov [SP+4], R1
  mov R1, [BP-7]
  mov [SP+5], R1
  call __function_define_region
  mov R0, [BP-1]
  mov R1, R0
  iadd R1, 1
  mov [BP-1], R1
  mov R0, [BP-2]
  mov R1, [BP-8]
  iadd R0, R1
  mov [BP-2], R0
  mov R0, [BP-4]
  mov R1, [BP-8]
  iadd R0, R1
  mov [BP-4], R0
  mov R0, [BP-6]
  mov R1, [BP-8]
  iadd R0, R1
  mov [BP-6], R0
__for_216_continue:
  mov R0, [BP-11]
  iadd R0, 1
  mov [BP-11], R0
  jmp __for_216_start
__for_216_end:
  mov R0, [BP-3]
  mov R1, [BP-9]
  iadd R0, R1
  mov [BP-3], R0
  mov R0, [BP-5]
  mov R1, [BP-9]
  iadd R0, R1
  mov [BP-5], R0
  mov R0, [BP-7]
  mov R1, [BP-9]
  iadd R0, R1
  mov [BP-7], R0
  mov R0, [BP+3]
  mov [BP-2], R0
  mov R0, [BP+5]
  mov [BP-4], R0
  mov R0, [BP+7]
  mov [BP-6], R0
__for_206_continue:
  mov R0, [BP-10]
  iadd R0, 1
  mov [BP-10], R0
  jmp __for_206_start
__for_206_end:
__function_define_region_matrix_return:
  mov SP, BP
  pop BP
  ret

__function_set_region_hotspot:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_RegionHotSpotX, R0
  mov R0, [BP+3]
  out GPU_RegionHotSpotY, R0
__function_set_region_hotspot_return:
  mov SP, BP
  pop BP
  ret

__function_set_multiply_color:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_MultiplyColor, R0
__function_set_multiply_color_return:
  mov SP, BP
  pop BP
  ret

__function_set_drawing_point:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingPointX, R0
  mov R0, [BP+3]
  out GPU_DrawingPointY, R0
__function_set_drawing_point_return:
  mov SP, BP
  pop BP
  ret

__function_set_drawing_scale:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingScaleX, R0
  mov R0, [BP+3]
  out GPU_DrawingScaleY, R0
__function_set_drawing_scale_return:
  mov SP, BP
  pop BP
  ret

__function_set_drawing_angle:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingAngle, R0
__function_set_drawing_angle_return:
  mov SP, BP
  pop BP
  ret

__function_set_blending_mode:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_ActiveBlending, R0
__function_set_blending_mode_return:
  mov SP, BP
  pop BP
  ret

__function_get_multiply_color:
  push BP
  mov BP, SP
  in R0, GPU_MultiplyColor
__function_get_multiply_color_return:
  mov SP, BP
  pop BP
  ret

__function_get_drawing_point:
  push BP
  mov BP, SP
  push R1
  in R0, GPU_DrawingPointX
  mov R1, [BP+2]
  mov [R1], R0
  in R0, GPU_DrawingPointY
  mov R1, [BP+3]
  mov [R1], R0
__function_get_drawing_point_return:
  mov SP, BP
  pop BP
  ret

__function_get_drawing_scale:
  push BP
  mov BP, SP
  push R1
  in R0, GPU_DrawingScaleX
  mov R1, [BP+2]
  mov [R1], R0
  in R0, GPU_DrawingScaleY
  mov R1, [BP+3]
  mov [R1], R0
__function_get_drawing_scale_return:
  mov SP, BP
  pop BP
  ret

__function_get_drawing_angle:
  push BP
  mov BP, SP
  in R0, GPU_DrawingAngle
__function_get_drawing_angle_return:
  mov SP, BP
  pop BP
  ret

__function_get_blending_mode:
  push BP
  mov BP, SP
  in R0, GPU_ActiveBlending
__function_get_blending_mode_return:
  mov SP, BP
  pop BP
  ret

__function_clear_screen:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_ClearColor, R0
  out GPU_Command, GPUCommand_ClearScreen
__function_clear_screen_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region:
  push BP
  mov BP, SP
  out GPU_Command, GPUCommand_DrawRegion
__function_draw_region_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_at:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingPointX, R0
  mov R0, [BP+3]
  out GPU_DrawingPointY, R0
  out GPU_Command, GPUCommand_DrawRegion
__function_draw_region_at_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_zoomed:
  push BP
  mov BP, SP
  out GPU_Command, GPUCommand_DrawRegionZoomed
__function_draw_region_zoomed_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_zoomed_at:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingPointX, R0
  mov R0, [BP+3]
  out GPU_DrawingPointY, R0
  out GPU_Command, GPUCommand_DrawRegionZoomed
__function_draw_region_zoomed_at_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_rotated:
  push BP
  mov BP, SP
  out GPU_Command, GPUCommand_DrawRegionRotated
__function_draw_region_rotated_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_rotated_at:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingPointX, R0
  mov R0, [BP+3]
  out GPU_DrawingPointY, R0
  out GPU_Command, GPUCommand_DrawRegionRotated
__function_draw_region_rotated_at_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_rotozoomed:
  push BP
  mov BP, SP
  out GPU_Command, GPUCommand_DrawRegionRotozoomed
__function_draw_region_rotozoomed_return:
  mov SP, BP
  pop BP
  ret

__function_draw_region_rotozoomed_at:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out GPU_DrawingPointX, R0
  mov R0, [BP+3]
  out GPU_DrawingPointY, R0
  out GPU_Command, GPUCommand_DrawRegionRotozoomed
__function_draw_region_rotozoomed_at_return:
  mov SP, BP
  pop BP
  ret

__function_print_at:
  push BP
  mov BP, SP
  isub SP, 4
  call __function_get_selected_texture
  mov [BP-1], R0
  mov R1, -1
  mov [SP], R1
  call __function_select_texture
  mov R0, [BP+2]
  mov [BP-2], R0
__while_361_start:
__while_361_continue:
  mov R0, [BP+4]
  mov R0, [R0]
  cib R0
  jf R0, __while_361_end
  mov R1, [BP+4]
  mov R1, [R1]
  mov [SP], R1
  call __function_select_region
  mov R1, [BP+2]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  call __function_draw_region_at
  mov R0, [BP+2]
  iadd R0, 10
  mov [BP+2], R0
__if_374_start:
  mov R0, [BP+4]
  mov R0, [R0]
  ieq R0, 10
  jf R0, __if_374_end
  mov R0, [BP-2]
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 20
  mov [BP+3], R0
__if_374_end:
  mov R0, [BP+4]
  iadd R0, 1
  mov [BP+4], R0
  jmp __while_361_start
__while_361_end:
  mov R1, [BP-1]
  mov [SP], R1
  call __function_select_texture
__function_print_at_return:
  mov SP, BP
  pop BP
  ret

__function_print:
  push BP
  mov BP, SP
  isub SP, 5
  lea R1, [BP-1]
  mov [SP], R1
  lea R1, [BP-2]
  mov [SP+1], R1
  call __function_get_drawing_point
  mov R1, [BP-1]
  mov [SP], R1
  mov R1, [BP-2]
  mov [SP+1], R1
  mov R1, [BP+2]
  mov [SP+2], R1
  call __function_print_at
__function_print_return:
  mov SP, BP
  pop BP
  ret

__function_select_gamepad:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out INP_SelectedGamepad, R0
__function_select_gamepad_return:
  mov SP, BP
  pop BP
  ret

__function_get_selected_gamepad:
  push BP
  mov BP, SP
  in R0, INP_SelectedGamepad
__function_get_selected_gamepad_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_is_connected:
  push BP
  mov BP, SP
  in R0, INP_GamepadConnected
__function_gamepad_is_connected_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_left:
  push BP
  mov BP, SP
  in R0, INP_GamepadLeft
__function_gamepad_left_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_right:
  push BP
  mov BP, SP
  in R0, INP_GamepadRight
__function_gamepad_right_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_up:
  push BP
  mov BP, SP
  in R0, INP_GamepadUp
__function_gamepad_up_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_down:
  push BP
  mov BP, SP
  in R0, INP_GamepadDown
__function_gamepad_down_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_direction:
  push BP
  mov BP, SP
__if_423_start:
  call __function_gamepad_left
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_423_else
  mov R0, -1
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_423_end
__if_423_else:
__if_432_start:
  call __function_gamepad_right
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_432_else
  mov R0, 1
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_432_end
__if_432_else:
  mov R0, 0
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__if_432_end:
__if_423_end:
__if_444_start:
  call __function_gamepad_up
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_444_else
  mov R0, -1
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_444_end
__if_444_else:
__if_453_start:
  call __function_gamepad_down
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_453_else
  mov R0, 1
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_453_end
__if_453_else:
  mov R0, 0
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
__if_453_end:
__if_444_end:
__function_gamepad_direction_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_direction_normalized:
  push BP
  mov BP, SP
__if_468_start:
  call __function_gamepad_left
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_468_else
  mov R0, -1.000000
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_468_end
__if_468_else:
__if_477_start:
  call __function_gamepad_right
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_477_else
  mov R0, 1.000000
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_477_end
__if_477_else:
  mov R0, 0.000000
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__if_477_end:
__if_468_end:
__if_489_start:
  call __function_gamepad_up
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_489_else
  mov R0, -1.000000
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_489_end
__if_489_else:
__if_498_start:
  call __function_gamepad_down
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_498_else
  mov R0, 1.000000
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  jmp __if_498_end
__if_498_else:
  mov R0, 0.000000
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
__if_498_end:
__if_489_end:
__if_510_start:
  mov R0, [BP+2]
  mov R0, [R0]
  cfb R0
  jf R0, __LogicalAnd_ShortCircuit_513
  mov R1, [BP+3]
  mov R1, [R1]
  cfb R1
  and R0, R1
__LogicalAnd_ShortCircuit_513:
  jf R0, __if_510_end
  mov R0, [BP+2]
  mov R0, [R0]
  fmul R0, 0.707107
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+3]
  mov R0, [R0]
  fmul R0, 0.707107
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
__if_510_end:
__function_gamepad_direction_normalized_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_a:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonA
__function_gamepad_button_a_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_b:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonB
__function_gamepad_button_b_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_x:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonX
__function_gamepad_button_x_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_y:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonY
__function_gamepad_button_y_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_l:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonL
__function_gamepad_button_l_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_r:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonR
__function_gamepad_button_r_return:
  mov SP, BP
  pop BP
  ret

__function_gamepad_button_start:
  push BP
  mov BP, SP
  in R0, INP_GamepadButtonStart
__function_gamepad_button_start_return:
  mov SP, BP
  pop BP
  ret

__function_get_cycle_counter:
  push BP
  mov BP, SP
  in R0, TIM_CycleCounter
__function_get_cycle_counter_return:
  mov SP, BP
  pop BP
  ret

__function_get_frame_counter:
  push BP
  mov BP, SP
  in R0, TIM_FrameCounter
__function_get_frame_counter_return:
  mov SP, BP
  pop BP
  ret

__function_get_date:
  push BP
  mov BP, SP
  in R0, TIM_CurrentDate
__function_get_date_return:
  mov SP, BP
  pop BP
  ret

__function_get_time:
  push BP
  mov BP, SP
  in R0, TIM_CurrentTime
__function_get_time_return:
  mov SP, BP
  pop BP
  ret

__function_translate_date:
  push BP
  mov BP, SP
  isub SP, 15
  mov R0, [BP+2]
  shl R0, -16
  mov R1, [BP+3]
  mov [R1], R0
  mov R0, [BP+2]
  and R0, 65535
  mov [BP-1], R0
  mov R0, 31
  mov [BP-13], R0
  mov R0, 28
  mov [BP-12], R0
  mov R0, 31
  mov [BP-11], R0
  mov R0, 30
  mov [BP-10], R0
  mov R0, 31
  mov [BP-9], R0
  mov R0, 30
  mov [BP-8], R0
  mov R0, 31
  mov [BP-7], R0
  mov R0, 31
  mov [BP-6], R0
  mov R0, 30
  mov [BP-5], R0
  mov R0, 31
  mov [BP-4], R0
  mov R0, 30
  mov [BP-3], R0
  mov R0, 31
  mov [BP-2], R0
  mov R1, [BP+3]
  mov R0, [R1]
  imod R0, 4
  ieq R0, 0
  jf R0, __LogicalAnd_ShortCircuit_594
  mov R2, [BP+3]
  mov R1, [R2]
  imod R1, 100
  ine R1, 0
  and R0, R1
__LogicalAnd_ShortCircuit_594:
  mov [BP-14], R0
__if_603_start:
  mov R0, [BP-14]
  jf R0, __if_603_end
  mov R0, 29
  mov [BP-12], R0
__if_603_end:
  mov R0, 1
  mov R1, [BP+3]
  iadd R1, 1
  mov [R1], R0
  mov R0, 0
  mov [BP-15], R0
__for_614_start:
  mov R0, [BP-15]
  ilt R0, 11
  jf R0, __for_614_end
__if_624_start:
  mov R0, [BP-1]
  lea R1, [BP-13]
  mov R2, [BP-15]
  iadd R1, R2
  mov R1, [R1]
  ilt R0, R1
  jf R0, __if_624_end
  mov R0, [BP-1]
  iadd R0, 1
  mov R1, [BP+3]
  iadd R1, 2
  mov [R1], R0
  jmp __function_translate_date_return
__if_624_end:
  mov R0, [BP-1]
  lea R1, [BP-13]
  mov R2, [BP-15]
  iadd R1, R2
  mov R1, [R1]
  isub R0, R1
  mov [BP-1], R0
  mov R2, [BP+3]
  iadd R2, 1
  mov R0, [R2]
  mov R1, R0
  iadd R1, 1
  mov [R2], R1
__for_614_continue:
  mov R0, [BP-15]
  iadd R0, 1
  mov [BP-15], R0
  jmp __for_614_start
__for_614_end:
  mov R0, 12
  mov R1, [BP+3]
  iadd R1, 1
  mov [R1], R0
  mov R0, [BP-1]
  iadd R0, 1
  mov R1, [BP+3]
  iadd R1, 2
  mov [R1], R0
__function_translate_date_return:
  mov SP, BP
  pop BP
  ret

__function_translate_time:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  idiv R0, 3600
  mov R1, [BP+3]
  mov [R1], R0
  mov R0, [BP+2]
  imod R0, 3600
  idiv R0, 60
  mov R1, [BP+3]
  iadd R1, 1
  mov [R1], R0
  mov R0, [BP+2]
  imod R0, 60
  mov R1, [BP+3]
  iadd R1, 2
  mov [R1], R0
__function_translate_time_return:
  mov SP, BP
  pop BP
  ret

__function_end_frame:
  push BP
  mov BP, SP
  wait
__function_end_frame_return:
  mov SP, BP
  pop BP
  ret

__function_sleep:
  push BP
  mov BP, SP
  isub SP, 2
  call __function_get_frame_counter
  mov [BP-1], R0
  mov R0, [BP-1]
  mov R1, [BP+2]
  iadd R0, R1
  mov [BP-2], R0
__while_699_start:
__while_699_continue:
  call __function_get_frame_counter
  mov R1, R0
  mov R2, [BP-2]
  ilt R1, R2
  mov R0, R1
  jf R0, __while_699_end
  call __function_end_frame
  jmp __while_699_start
__while_699_end:
__function_sleep_return:
  mov SP, BP
  pop BP
  ret

__function_fmod:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  fmod R0, R1
  pop R1
__function_fmod_return:
  mov SP, BP
  pop BP
  ret

__function_min:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  imin R0, R1
  pop R1
__function_min_return:
  mov SP, BP
  pop BP
  ret

__function_max:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  imax R0, R1
  pop R1
__function_max_return:
  mov SP, BP
  pop BP
  ret

__function_abs:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  iabs R0
__function_abs_return:
  mov SP, BP
  pop BP
  ret

__function_fmin:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  fmin R0, R1
  pop R1
__function_fmin_return:
  mov SP, BP
  pop BP
  ret

__function_fmax:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  fmax R0, R1
  pop R1
__function_fmax_return:
  mov SP, BP
  pop BP
  ret

__function_fabs:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  fabs R0
__function_fabs_return:
  mov SP, BP
  pop BP
  ret

__function_floor:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  flr R0
__function_floor_return:
  mov SP, BP
  pop BP
  ret

__function_ceil:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  ceil R0
__function_ceil_return:
  mov SP, BP
  pop BP
  ret

__function_round:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  round R0
__function_round_return:
  mov SP, BP
  pop BP
  ret

__function_sin:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  sin R0
__function_sin_return:
  mov SP, BP
  pop BP
  ret

__function_cos:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  fadd R0, 1.570796
  sin R0
__function_cos_return:
  mov SP, BP
  pop BP
  ret

__function_tan:
  push BP
  mov BP, SP
  push R1
  mov R1, [BP+2]
  mov R0, R1
  sin R0
  fadd R1, 1.570796
  sin R1
  fdiv R0, R1
  pop R1
__function_tan_return:
  mov SP, BP
  pop BP
  ret

__function_asin:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  acos R0
  fsgn R0
  fadd R0, 1.570796
__function_asin_return:
  mov SP, BP
  pop BP
  ret

__function_acos:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  acos R0
__function_acos_return:
  mov SP, BP
  pop BP
  ret

__function_atan2:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  atan2 R0, R1
  pop R1
__function_atan2_return:
  mov SP, BP
  pop BP
  ret

__function_sqrt:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, 0.5
  pow R0, R1
  pop R1
__function_sqrt_return:
  mov SP, BP
  pop BP
  ret

__function_pow:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  mov R1, [BP+3]
  pow R0, R1
  pop R1
__function_pow_return:
  mov SP, BP
  pop BP
  ret

__function_exp:
  push BP
  mov BP, SP
  push R1
  mov R0, 2.718282
  mov R1, [BP+2]
  pow R0, R1
  pop R1
__function_exp_return:
  mov SP, BP
  pop BP
  ret

__function_log:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  log R0
__function_log_return:
  mov SP, BP
  pop BP
  ret

__function_isdigit:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  ige R0, 48
  jf R0, __LogicalAnd_ShortCircuit_864
  mov R1, [BP+2]
  ile R1, 57
  and R0, R1
__LogicalAnd_ShortCircuit_864:
__function_isdigit_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isxdigit:
  push BP
  mov BP, SP
  push R1
__if_869_start:
  mov R0, [BP+2]
  ige R0, 48
  jf R0, __LogicalAnd_ShortCircuit_874
  mov R1, [BP+2]
  ile R1, 57
  and R0, R1
__LogicalAnd_ShortCircuit_874:
  jf R0, __if_869_end
  mov R0, 1
  jmp __function_isxdigit_return
__if_869_end:
__if_879_start:
  mov R0, [BP+2]
  ige R0, 97
  jf R0, __LogicalAnd_ShortCircuit_884
  mov R1, [BP+2]
  ile R1, 102
  and R0, R1
__LogicalAnd_ShortCircuit_884:
  jf R0, __if_879_end
  mov R0, 1
  jmp __function_isxdigit_return
__if_879_end:
  mov R0, [BP+2]
  ige R0, 65
  jf R0, __LogicalAnd_ShortCircuit_895
  mov R1, [BP+2]
  ile R1, 70
  and R0, R1
__LogicalAnd_ShortCircuit_895:
__function_isxdigit_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isalpha:
  push BP
  mov BP, SP
  push R1
__if_900_start:
  mov R0, [BP+2]
  ige R0, 97
  jf R0, __LogicalAnd_ShortCircuit_905
  mov R1, [BP+2]
  ile R1, 122
  and R0, R1
__LogicalAnd_ShortCircuit_905:
  jf R0, __if_900_end
  mov R0, 1
  jmp __function_isalpha_return
__if_900_end:
  mov R0, [BP+2]
  ige R0, 65
  jf R0, __LogicalAnd_ShortCircuit_916
  mov R1, [BP+2]
  ile R1, 90
  and R0, R1
__LogicalAnd_ShortCircuit_916:
__function_isalpha_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isascii:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  ige R0, 0
  jf R0, __LogicalAnd_ShortCircuit_927
  mov R1, [BP+2]
  ile R1, 127
  and R0, R1
__LogicalAnd_ShortCircuit_927:
__function_isascii_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isalphanum:
  push BP
  mov BP, SP
  push R1
__if_932_start:
  mov R0, [BP+2]
  ige R0, 48
  jf R0, __LogicalAnd_ShortCircuit_937
  mov R1, [BP+2]
  ile R1, 57
  and R0, R1
__LogicalAnd_ShortCircuit_937:
  jf R0, __if_932_end
  mov R0, 1
  jmp __function_isalphanum_return
__if_932_end:
__if_942_start:
  mov R0, [BP+2]
  ige R0, 97
  jf R0, __LogicalAnd_ShortCircuit_947
  mov R1, [BP+2]
  ile R1, 122
  and R0, R1
__LogicalAnd_ShortCircuit_947:
  jf R0, __if_942_end
  mov R0, 1
  jmp __function_isalphanum_return
__if_942_end:
  mov R0, [BP+2]
  ige R0, 65
  jf R0, __LogicalAnd_ShortCircuit_958
  mov R1, [BP+2]
  ile R1, 90
  and R0, R1
__LogicalAnd_ShortCircuit_958:
__function_isalphanum_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_islower:
  push BP
  mov BP, SP
  push R1
__if_963_start:
  mov R0, [BP+2]
  ige R0, 97
  jf R0, __LogicalAnd_ShortCircuit_968
  mov R1, [BP+2]
  ile R1, 122
  and R0, R1
__LogicalAnd_ShortCircuit_968:
  jf R0, __if_963_end
  mov R0, 1
  jmp __function_islower_return
__if_963_end:
  mov R0, [BP+2]
  ige R0, 224
  jf R0, __LogicalAnd_ShortCircuit_979
  mov R1, [BP+2]
  ile R1, 254
  and R0, R1
__LogicalAnd_ShortCircuit_979:
  jf R0, __LogicalAnd_ShortCircuit_983
  mov R1, [BP+2]
  ine R1, 247
  and R0, R1
__LogicalAnd_ShortCircuit_983:
__function_islower_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isupper:
  push BP
  mov BP, SP
  push R1
__if_988_start:
  mov R0, [BP+2]
  ige R0, 65
  jf R0, __LogicalAnd_ShortCircuit_993
  mov R1, [BP+2]
  ile R1, 90
  and R0, R1
__LogicalAnd_ShortCircuit_993:
  jf R0, __if_988_end
  mov R0, 1
  jmp __function_isupper_return
__if_988_end:
  mov R0, [BP+2]
  ige R0, 192
  jf R0, __LogicalAnd_ShortCircuit_1004
  mov R1, [BP+2]
  ile R1, 222
  and R0, R1
__LogicalAnd_ShortCircuit_1004:
  jf R0, __LogicalAnd_ShortCircuit_1008
  mov R1, [BP+2]
  ine R1, 215
  and R0, R1
__LogicalAnd_ShortCircuit_1008:
__function_isupper_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_isspace:
  push BP
  mov BP, SP
  push R1
  mov R0, [BP+2]
  ieq R0, 32
  jt R0, __LogicalOr_ShortCircuit_1019
  mov R1, [BP+2]
  ieq R1, 10
  or R0, R1
__LogicalOr_ShortCircuit_1019:
  jt R0, __LogicalOr_ShortCircuit_1023
  mov R1, [BP+2]
  ieq R1, 13
  or R0, R1
__LogicalOr_ShortCircuit_1023:
  jt R0, __LogicalOr_ShortCircuit_1027
  mov R1, [BP+2]
  ieq R1, 9
  or R0, R1
__LogicalOr_ShortCircuit_1027:
__function_isspace_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_tolower:
  push BP
  mov BP, SP
  push R1
  push R2
  isub SP, 1
__if_1032_start:
  mov R2, [BP+2]
  mov [SP], R2
  call __function_isupper
  mov R1, R0
  bnot R1
  mov R0, R1
  jf R0, __if_1032_end
  mov R0, [BP+2]
  jmp __function_tolower_return
__if_1032_end:
  mov R0, [BP+2]
  iadd R0, 32
__function_tolower_return:
  iadd SP, 1
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_toupper:
  push BP
  mov BP, SP
  push R1
  push R2
  isub SP, 1
__if_1044_start:
  mov R2, [BP+2]
  mov [SP], R2
  call __function_islower
  mov R1, R0
  bnot R1
  mov R0, R1
  jf R0, __if_1044_end
  mov R0, [BP+2]
  jmp __function_toupper_return
__if_1044_end:
  mov R0, [BP+2]
  isub R0, 32
__function_toupper_return:
  iadd SP, 1
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_strlen:
  push BP
  mov BP, SP
  isub SP, 1
  push R1
  mov R0, [BP+2]
  mov [BP-1], R0
__while_1059_start:
__while_1059_continue:
  mov R0, [BP+2]
  mov R0, [R0]
  cib R0
  jf R0, __while_1059_end
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  jmp __while_1059_start
__while_1059_end:
  mov R0, [BP+2]
  mov R1, [BP-1]
  isub R0, R1
__function_strlen_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_strcmp:
  push BP
  mov BP, SP
  push R1
__while_1071_start:
__while_1071_continue:
  mov R0, [BP+2]
  mov R0, [R0]
  cib R0
  jf R0, __LogicalAnd_ShortCircuit_1074
  mov R1, [BP+3]
  mov R1, [R1]
  cib R1
  and R0, R1
__LogicalAnd_ShortCircuit_1074:
  jf R0, __while_1071_end
__if_1078_start:
  mov R0, [BP+2]
  mov R0, [R0]
  mov R1, [BP+3]
  mov R1, [R1]
  ine R0, R1
  jf R0, __if_1078_end
  jmp __while_1071_end
__if_1078_end:
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  jmp __while_1071_start
__while_1071_end:
  mov R0, [BP+2]
  mov R0, [R0]
  mov R1, [BP+3]
  mov R1, [R1]
  isub R0, R1
__function_strcmp_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_strncmp:
  push BP
  mov BP, SP
  push R1
__if_1099_start:
  mov R0, [BP+4]
  ilt R0, 1
  jf R0, __if_1099_end
  mov R0, 0
  jmp __function_strncmp_return
__if_1099_end:
__while_1105_start:
__while_1105_continue:
  mov R0, [BP+2]
  mov R0, [R0]
  cib R0
  jf R0, __LogicalAnd_ShortCircuit_1108
  mov R1, [BP+3]
  mov R1, [R1]
  cib R1
  and R0, R1
__LogicalAnd_ShortCircuit_1108:
  jf R0, __while_1105_end
  mov R0, [BP+4]
  isub R0, 1
  mov [BP+4], R0
__if_1114_start:
  mov R0, [BP+4]
  ile R0, 0
  jf R0, __if_1114_end
  jmp __while_1105_end
__if_1114_end:
__if_1119_start:
  mov R0, [BP+2]
  mov R0, [R0]
  mov R1, [BP+3]
  mov R1, [R1]
  ine R0, R1
  jf R0, __if_1119_end
  jmp __while_1105_end
__if_1119_end:
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  jmp __while_1105_start
__while_1105_end:
  mov R0, [BP+2]
  mov R0, [R0]
  mov R1, [BP+3]
  mov R1, [R1]
  isub R0, R1
__function_strncmp_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_strcpy:
  push BP
  mov BP, SP
__while_1139_start:
__while_1139_continue:
  mov R0, [BP+3]
  mov R0, [R0]
  cib R0
  jf R0, __while_1139_end
  mov R0, [BP+3]
  mov R0, [R0]
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  jmp __while_1139_start
__while_1139_end:
  mov R0, 0
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__function_strcpy_return:
  mov SP, BP
  pop BP
  ret

__function_strncpy:
  push BP
  mov BP, SP
__if_1160_start:
  mov R0, [BP+4]
  ilt R0, 1
  jf R0, __if_1160_end
  jmp __function_strncpy_return
__if_1160_end:
__while_1165_start:
__while_1165_continue:
  mov R0, [BP+3]
  mov R0, [R0]
  cib R0
  jf R0, __while_1165_end
  mov R0, [BP+3]
  mov R0, [R0]
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  mov R0, [BP+4]
  isub R0, 1
  mov [BP+4], R0
__if_1180_start:
  mov R0, [BP+4]
  ile R0, 0
  jf R0, __if_1180_end
  jmp __while_1165_end
__if_1180_end:
  jmp __while_1165_start
__while_1165_end:
  mov R0, 0
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__function_strncpy_return:
  mov SP, BP
  pop BP
  ret

__function_strcat:
  push BP
  mov BP, SP
__while_1192_start:
__while_1192_continue:
  mov R0, [BP+2]
  mov R0, [R0]
  cib R0
  jf R0, __while_1192_end
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  jmp __while_1192_start
__while_1192_end:
__while_1197_start:
__while_1197_continue:
  mov R0, [BP+3]
  mov R0, [R0]
  cib R0
  jf R0, __while_1197_end
  mov R0, [BP+3]
  mov R0, [R0]
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  jmp __while_1197_start
__while_1197_end:
  mov R0, 0
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__function_strcat_return:
  mov SP, BP
  pop BP
  ret

__function_strncat:
  push BP
  mov BP, SP
__if_1218_start:
  mov R0, [BP+4]
  ilt R0, 1
  jf R0, __if_1218_end
  jmp __function_strncat_return
__if_1218_end:
__while_1223_start:
__while_1223_continue:
  mov R0, [BP+2]
  mov R0, [R0]
  cib R0
  jf R0, __while_1223_end
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  jmp __while_1223_start
__while_1223_end:
__while_1228_start:
__while_1228_continue:
  mov R0, [BP+3]
  mov R0, [R0]
  cib R0
  jf R0, __while_1228_end
  mov R0, [BP+3]
  mov R0, [R0]
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+2]
  iadd R0, 1
  mov [BP+2], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  mov R0, [BP+4]
  isub R0, 1
  mov [BP+4], R0
__if_1243_start:
  mov R0, [BP+4]
  ile R0, 0
  jf R0, __if_1243_end
  jmp __while_1228_end
__if_1243_end:
  jmp __while_1228_start
__while_1228_end:
  mov R0, 0
  lea R1, [BP+2]
  mov R1, [R1]
  mov [R1], R0
__function_strncat_return:
  mov SP, BP
  pop BP
  ret

__function_itoa:
  push BP
  mov BP, SP
  isub SP, 51
  lea DR, [BP-17]
  mov SR, __literal_string_1261
  mov CR, 17
  movs
__if_1265_start:
  mov R0, [BP+4]
  ilt R0, 2
  jt R0, __LogicalOr_ShortCircuit_1270
  mov R1, [BP+4]
  igt R1, 16
  or R0, R1
__LogicalOr_ShortCircuit_1270:
  jf R0, __if_1265_end
  jmp __function_itoa_return
__if_1265_end:
__if_1274_start:
  mov R0, [BP+4]
  ieq R0, 10
  jf R0, __LogicalAnd_ShortCircuit_1279
  mov R1, [BP+2]
  ilt R1, 0
  and R0, R1
__LogicalAnd_ShortCircuit_1279:
  jf R0, __if_1274_end
  mov R0, 45
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  mov R0, [BP+2]
  isgn R0
  mov [BP+2], R0
__if_1274_end:
  lea R0, [BP-50]
  mov [BP-51], R0
__do_1296_start:
  lea R0, [BP-17]
  mov R1, [BP+2]
  mov R2, [BP+4]
  imod R1, R2
  iadd R0, R1
  mov R0, [R0]
  lea R1, [BP-51]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP-51]
  iadd R0, 1
  mov [BP-51], R0
  mov R0, [BP+2]
  mov R1, [BP+4]
  idiv R0, R1
  mov [BP+2], R0
__do_1296_continue:
  mov R0, [BP+2]
  cib R0
  jt R0, __do_1296_start
__do_1296_end:
__do_1312_start:
  mov R0, [BP-51]
  isub R0, 1
  mov [BP-51], R0
  mov R0, [BP-51]
  mov R0, [R0]
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
__do_1312_continue:
  mov R0, [BP-51]
  lea R1, [BP-50]
  ine R0, R1
  jt R0, __do_1312_start
__do_1312_end:
  mov R0, 0
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
__function_itoa_return:
  mov SP, BP
  pop BP
  ret

__function_ftoa:
  push BP
  mov BP, SP
  isub SP, 14
__if_1336_start:
  mov R0, [BP+2]
  flt R0, 0.000000
  jf R0, __if_1336_end
  mov R0, 45
  lea R1, [BP+3]
  mov R1, [R1]
  mov [R1], R0
  mov R0, [BP+3]
  iadd R0, 1
  mov [BP+3], R0
  mov R0, [BP+2]
  fsgn R0
  mov [BP+2], R0
__if_1336_end:
  mov R0, [BP+2]
  flr R0
  cfi R0
  mov [BP-1], R0
  push R1
  mov R0, [BP+2]
  mov R1, [BP-1]
  cif R1
  fsub R0, R1
  fmul R0, 100000.0
  round R0
  cfi R0
  mov [BP-2], R0
  pop R1
  mov R1, [BP-1]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  mov R1, 10
  mov [SP+2], R1
  call __function_itoa
__if_1365_start:
  mov R0, [BP-2]
  bnot R0
  jf R0, __if_1365_end
  jmp __function_ftoa_return
__if_1365_end:
  mov R1, [BP+3]
  mov [SP], R1
  mov R1, __literal_string_1371
  mov [SP+1], R1
  call __function_strcat
  mov R0, [BP-2]
  mov [BP-3], R0
__while_1375_start:
__while_1375_continue:
  mov R0, [BP-3]
  ilt R0, 10000
  jf R0, __while_1375_end
  mov R1, [BP+3]
  mov [SP], R1
  mov R1, __literal_string_1382
  mov [SP+1], R1
  call __function_strcat
  mov R0, [BP-3]
  imul R0, 10
  mov [BP-3], R0
  jmp __while_1375_start
__while_1375_end:
__while_1386_start:
__while_1386_continue:
  mov R0, [BP-2]
  imod R0, 10
  bnot R0
  jf R0, __while_1386_end
  mov R0, [BP-2]
  idiv R0, 10
  mov [BP-2], R0
  jmp __while_1386_start
__while_1386_end:
  mov R1, [BP-2]
  mov [SP], R1
  lea R1, [BP-11]
  mov [SP+1], R1
  mov R1, 10
  mov [SP+2], R1
  call __function_itoa
  mov R1, [BP+3]
  mov [SP], R1
  lea R1, [BP-11]
  mov [SP+1], R1
  call __function_strcat
__function_ftoa_return:
  mov SP, BP
  pop BP
  ret

__function_is_over_carrier:
  push BP
  mov BP, SP
  isub SP, 4
  push R1
  mov R0, 1038.000000
  mov [BP-1], R0
  mov R0, 1255.000000
  mov [BP-2], R0
  mov R0, [global_airplane_x]
  mov R1, [BP-1]
  fsub R0, R1
  mov [BP-3], R0
  mov R0, [global_airplane_y]
  fsub R0, 1000.000000
  mov [BP-4], R0
  mov R0, [BP-3]
  fge R0, -38.000000
  jf R0, __LogicalAnd_ShortCircuit_1469
  mov R1, [BP-3]
  fle R1, 38.000000
  and R0, R1
__LogicalAnd_ShortCircuit_1469:
  jf R0, __LogicalAnd_ShortCircuit_1475
  mov R1, [BP-4]
  fge R1, 0.000000
  and R0, R1
__LogicalAnd_ShortCircuit_1475:
  jf R0, __LogicalAnd_ShortCircuit_1479
  mov R1, [BP-4]
  fle R1, 255.000000
  and R0, R1
__LogicalAnd_ShortCircuit_1479:
__function_is_over_carrier_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_render_fuel_gauge:
  push BP
  mov BP, SP
  isub SP, 8
  mov R1, -1
  mov [SP], R1
  call __function_select_texture
  mov R1, -16776961
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, 10
  mov [SP], R1
  mov R1, 10
  mov [SP+1], R1
  mov R1, __literal_string_1491
  mov [SP+2], R1
  call __function_print_at
  mov R0, 100
  mov [BP-1], R0
  mov R0, 10
  mov [BP-2], R0
  mov R0, [global_fuel]
  fdiv R0, 1000.000000
  mov R1, [BP-1]
  cif R1
  fmul R0, R1
  cfi R0
  mov [BP-3], R0
  mov R0, 0
  mov [BP-4], R0
__for_1508_start:
  mov R0, [BP-4]
  mov R1, [BP-1]
  ilt R0, R1
  jf R0, __for_1508_end
  mov R0, 0
  mov [BP-5], R0
__for_1518_start:
  mov R0, [BP-5]
  mov R1, [BP-2]
  ilt R0, R1
  jf R0, __for_1518_end
  mov R1, [BP-4]
  iadd R1, 60
  mov [SP], R1
  mov R1, [BP-5]
  iadd R1, 10
  mov [SP+1], R1
  call __function_draw_region_at
__for_1518_continue:
  mov R0, [BP-5]
  mov R1, R0
  iadd R1, 1
  mov [BP-5], R1
  jmp __for_1518_start
__for_1518_end:
__for_1508_continue:
  mov R0, [BP-4]
  mov R1, R0
  iadd R1, 1
  mov [BP-4], R1
  jmp __for_1508_start
__for_1508_end:
  mov R1, -1
  mov [SP], R1
  call __function_set_multiply_color
  mov R0, 0
  mov [BP-4], R0
__for_1537_start:
  mov R0, [BP-4]
  mov R1, [BP-3]
  ilt R0, R1
  jf R0, __for_1537_end
  mov R0, 0
  mov [BP-5], R0
__for_1547_start:
  mov R0, [BP-5]
  mov R1, [BP-2]
  ilt R0, R1
  jf R0, __for_1547_end
  mov R1, [BP-4]
  iadd R1, 60
  mov [SP], R1
  mov R1, [BP-5]
  iadd R1, 10
  mov [SP+1], R1
  call __function_draw_region_at
__for_1547_continue:
  mov R0, [BP-5]
  mov R1, R0
  iadd R1, 1
  mov [BP-5], R1
  jmp __for_1547_start
__for_1547_end:
__for_1537_continue:
  mov R0, [BP-4]
  mov R1, R0
  iadd R1, 1
  mov [BP-4], R1
  jmp __for_1537_start
__for_1537_end:
__function_render_fuel_gauge_return:
  mov SP, BP
  pop BP
  ret

__function_initialize_airplane:
  push BP
  mov BP, SP
  isub SP, 6
  mov R1, 0
  mov [SP], R1
  call __function_select_texture
  mov R1, 0
  mov [SP], R1
  call __function_select_region
  mov R1, 0
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 128
  mov [SP+2], R1
  mov R1, 91
  mov [SP+3], R1
  mov R1, 64
  mov [SP+4], R1
  mov R1, 45
  mov [SP+5], R1
  call __function_define_region
  mov R1, 1
  mov [SP], R1
  call __function_select_region
  mov R1, 128
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 256
  mov [SP+2], R1
  mov R1, 91
  mov [SP+3], R1
  mov R1, 192.000000
  cfi R1
  mov [SP+4], R1
  mov R1, 45
  mov [SP+5], R1
  call __function_define_region
  mov R1, 2
  mov [SP], R1
  call __function_select_region
  mov R1, 0
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 128
  mov [SP+2], R1
  mov R1, 91
  mov [SP+3], R1
  mov R1, 64
  mov [SP+4], R1
  mov R1, 45
  mov [SP+5], R1
  call __function_define_region
  mov R1, 3
  mov [SP], R1
  call __function_select_region
  mov R1, 128
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 256
  mov [SP+2], R1
  mov R1, 91
  mov [SP+3], R1
  mov R1, 192.000000
  cfi R1
  mov [SP+4], R1
  mov R1, 45
  mov [SP+5], R1
  call __function_define_region
  mov R0, 0
  mov [global_airplane_frame], R0
  mov R0, 0
  mov [global_anim_timer], R0
  call __function_reset_airplane
__function_initialize_airplane_return:
  mov SP, BP
  pop BP
  ret

__function_reset_airplane:
  push BP
  mov BP, SP
  mov R0, 998.000000
  mov [global_airplane_x], R0
  mov R0, 1020.000000
  mov [global_airplane_y], R0
  mov R0, 0.000000
  mov [global_airplane_angle], R0
  mov R0, 0.300000
  mov [global_airplane_scale], R0
  mov R0, 0.000000
  mov [global_airplane_velocity], R0
  mov R0, 1000.000000
  mov [global_fuel], R0
  mov R0, [global_airplane_x]
  fsub R0, 320.000000
  mov [global_camera_x], R0
  mov R0, [global_airplane_y]
  fsub R0, 180.000000
  mov [global_camera_y], R0
__function_reset_airplane_return:
  mov SP, BP
  pop BP
  ret

__function_update_airplane:
  push BP
  mov BP, SP
  isub SP, 5
  lea R1, [BP-1]
  mov [SP], R1
  lea R1, [BP-2]
  mov [SP+1], R1
  call __function_gamepad_direction
__if_1691_start:
  call __function_gamepad_left
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_1691_end
  mov R0, [global_airplane_angle]
  fsub R0, 0.050000
  mov [global_airplane_angle], R0
__if_1691_end:
__if_1698_start:
  call __function_gamepad_right
  mov R1, R0
  igt R1, 0
  mov R0, R1
  jf R0, __if_1698_end
  mov R0, [global_airplane_angle]
  fadd R0, 0.050000
  mov [global_airplane_angle], R0
__if_1698_end:
__if_1705_start:
  call __function_gamepad_up
  mov R1, R0
  igt R1, 0
  jf R1, __LogicalAnd_ShortCircuit_1710
  mov R2, [global_fuel]
  fgt R2, 0.000000
  and R1, R2
__LogicalAnd_ShortCircuit_1710:
  mov R0, R1
  jf R0, __if_1705_else
  mov R1, [global_airplane_x]
  mov R3, [global_airplane_angle]
  mov [SP], R3
  call __function_sin
  mov R2, R0
  fmul R2, 3.000000
  fadd R1, R2
  mov [global_airplane_x], R1
  mov R0, R1
  mov R1, [global_airplane_y]
  mov R3, [global_airplane_angle]
  mov [SP], R3
  call __function_cos
  mov R2, R0
  fmul R2, 3.000000
  fsub R1, R2
  mov [global_airplane_y], R1
  mov R0, R1
  mov R2, [global_airplane_scale]
  fadd R2, 0.010000
  mov [SP], R2
  mov R2, 0.200000
  mov [SP+1], R2
  mov R2, 1.000000
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_airplane_scale], R1
  mov R0, R1
  mov R0, [global_fuel]
  fsub R0, 0.500000
  mov [global_fuel], R0
  mov R0, [global_anim_timer]
  mov R1, R0
  iadd R1, 1
  mov [global_anim_timer], R1
__if_1739_start:
  mov R0, [global_anim_timer]
  ige R0, 10
  jf R0, __if_1739_end
  mov R0, 0
  mov [global_anim_timer], R0
  mov R0, [global_airplane_frame]
  isgn R0
  iadd R0, 1
  mov [global_airplane_frame], R0
__if_1739_end:
  jmp __if_1705_end
__if_1705_else:
  mov R2, [global_airplane_scale]
  fsub R2, 0.010000
  mov [SP], R2
  mov R2, 0.200000
  mov [SP+1], R2
  mov R2, 1.000000
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_airplane_scale], R1
  mov R0, R1
  mov R0, 0
  mov [global_airplane_frame], R0
  mov R0, 0
  mov [global_anim_timer], R0
__if_1705_end:
__if_1767_start:
  mov R0, [global_airplane_scale]
  fle R0, 0.500000
  jf R0, __if_1767_else
__if_1772_start:
  call __function_is_over_carrier
  mov R1, R0
  cib R1
  jt R1, __LogicalOr_ShortCircuit_1774
  mov R3, [global_airplane_x]
  mov [SP], R3
  mov R3, [global_airplane_y]
  mov [SP+1], R3
  call __function_is_over_island
  mov R2, R0
  cib R2
  or R1, R2
__LogicalOr_ShortCircuit_1774:
  mov R0, R1
  jf R0, __if_1772_else
  mov R0, 0.500000
  mov [global_airplane_scale], R0
__if_1782_start:
  call __function_is_over_carrier
  cib R0
  jf R0, __if_1782_end
  mov R2, [global_fuel]
  fadd R2, 2.000000
  mov [SP], R2
  mov R2, 0
  cif R2
  mov [SP+1], R2
  mov R2, 1000
  cif R2
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_fuel], R1
  mov R0, R1
__if_1782_end:
  jmp __if_1772_end
__if_1772_else:
__if_1793_start:
  mov R0, [global_airplane_scale]
  fle R0, 0.200000
  jf R0, __if_1793_end
  mov R0, 2
  mov [global_game_state], R0
__if_1793_end:
__if_1772_end:
  jmp __if_1767_end
__if_1767_else:
  mov R2, [global_airplane_scale]
  fsub R2, 0.010000
  mov [SP], R2
  mov R2, 0.200000
  mov [SP+1], R2
  mov R2, 1.000000
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_airplane_scale], R1
  mov R0, R1
__if_1767_end:
__if_1810_start:
  mov R1, [global_fuel]
  fle R1, 0.000000
  jf R1, __LogicalAnd_ShortCircuit_1815
  mov R2, [global_airplane_scale]
  fle R2, 0.200000
  and R1, R2
__LogicalAnd_ShortCircuit_1815:
  jf R1, __LogicalAnd_ShortCircuit_1820
  call __function_is_over_carrier
  mov R2, R0
  bnot R2
  and R1, R2
__LogicalAnd_ShortCircuit_1820:
  jf R1, __LogicalAnd_ShortCircuit_1825
  mov R3, [global_airplane_x]
  mov [SP], R3
  mov R3, [global_airplane_y]
  mov [SP+1], R3
  call __function_is_over_island
  mov R2, R0
  bnot R2
  and R1, R2
__LogicalAnd_ShortCircuit_1825:
  mov R0, R1
  jf R0, __if_1810_end
  mov R0, 2
  mov [global_game_state], R0
__if_1810_end:
  mov R2, [global_airplane_x]
  mov [SP], R2
  mov R2, 0
  cif R2
  mov [SP+1], R2
  mov R2, 2000
  cif R2
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_airplane_x], R1
  mov R0, R1
  mov R2, [global_airplane_y]
  mov [SP], R2
  mov R2, 0
  cif R2
  mov [SP+1], R2
  mov R2, 2000
  cif R2
  mov [SP+2], R2
  call __function_clamp
  mov R1, R0
  mov [global_airplane_y], R1
  mov R0, R1
  mov R0, [global_airplane_x]
  fsub R0, 320.000000
  mov [global_camera_x], R0
  mov R0, [global_airplane_y]
  fsub R0, 180.000000
  mov [global_camera_y], R0
__function_update_airplane_return:
  mov SP, BP
  pop BP
  ret

__function_render_airplane:
  push BP
  mov BP, SP
  isub SP, 2
  mov R1, [global_camera_x]
  mov [SP], R1
  mov R1, [global_camera_y]
  mov [SP+1], R1
  call __function_render_world
  mov R1, 0
  mov [SP], R1
  call __function_select_texture
  mov R1, [global_airplane_frame]
  iadd R1, 2
  mov [SP], R1
  call __function_select_region
  mov R1, 0x80000000
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, [global_airplane_scale]
  mov [SP], R1
  mov R1, [global_airplane_scale]
  mov [SP+1], R1
  call __function_set_drawing_scale
  mov R1, [global_airplane_angle]
  mov [SP], R1
  call __function_set_drawing_angle
  mov R1, [global_airplane_x]
  mov R2, [global_camera_x]
  fsub R1, R2
  fadd R1, 20.000000
  cfi R1
  mov [SP], R1
  mov R1, [global_airplane_y]
  mov R2, [global_camera_y]
  fsub R1, R2
  fadd R1, 20.000000
  cfi R1
  mov [SP+1], R1
  call __function_draw_region_rotozoomed_at
  mov R1, -1
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, [global_airplane_frame]
  mov [SP], R1
  call __function_select_region
  mov R1, [global_airplane_scale]
  mov [SP], R1
  mov R1, [global_airplane_scale]
  mov [SP+1], R1
  call __function_set_drawing_scale
  mov R1, [global_airplane_angle]
  mov [SP], R1
  call __function_set_drawing_angle
  mov R1, [global_airplane_x]
  mov R2, [global_camera_x]
  fsub R1, R2
  cfi R1
  mov [SP], R1
  mov R1, [global_airplane_y]
  mov R2, [global_camera_y]
  fsub R1, R2
  cfi R1
  mov [SP+1], R1
  call __function_draw_region_rotozoomed_at
  call __function_render_fuel_gauge
__function_render_airplane_return:
  mov SP, BP
  pop BP
  ret

__function_memset:
  push BP
  mov BP, SP
  mov CR, [BP+4]
  mov DR, [BP+2]
  mov SR, [BP+3]
  sets
__function_memset_return:
  mov SP, BP
  pop BP
  ret

__function_memcpy:
  push BP
  mov BP, SP
  mov CR, [BP+4]
  mov DR, [BP+2]
  mov SR, [BP+3]
  movs
__function_memcpy_return:
  mov SP, BP
  pop BP
  ret

__function_memcmp:
  push BP
  mov BP, SP
  mov CR, [BP+4]
  mov DR, [BP+2]
  mov SR, [BP+3]
  cmps R0
__function_memcmp_return:
  mov SP, BP
  pop BP
  ret

__function_merge_free_malloc_blocks:
  push BP
  mov BP, SP
  isub SP, 2
  mov R1, [BP+2]
  mov R0, [R1]
  mov [BP-1], R0
  mov R1, [BP+2]
  iadd R1, 1
  mov R0, [R1]
  mov [BP-2], R0
__if_1969_start:
  mov R0, [BP-2]
  ine R0, -1
  jf R0, __LogicalAnd_ShortCircuit_1971
  mov R2, [BP-2]
  iadd R2, 3
  mov R1, [R2]
  and R0, R1
__LogicalAnd_ShortCircuit_1971:
  jf R0, __if_1969_end
  mov R1, [BP-2]
  iadd R1, 1
  mov R0, [R1]
  mov R1, [BP+2]
  iadd R1, 1
  mov [R1], R0
__if_1980_start:
  mov R1, [BP-2]
  iadd R1, 1
  mov R0, [R1]
  ine R0, -1
  jf R0, __if_1980_end
  mov R0, [BP+2]
  mov R2, [BP-2]
  iadd R2, 1
  mov R1, [R2]
  mov [R1], R0
__if_1980_end:
  mov R1, [BP+2]
  iadd R1, 2
  mov R0, [R1]
  mov R2, [BP-2]
  iadd R2, 2
  mov R1, [R2]
  iadd R1, 4
  iadd R0, R1
  mov R1, [BP+2]
  iadd R1, 2
  mov [R1], R0
  mov R1, [BP+2]
  iadd R1, 1
  mov R0, [R1]
  mov [BP-2], R0
__if_1969_end:
__if_1999_start:
  mov R0, [BP-1]
  ine R0, -1
  jf R0, __LogicalAnd_ShortCircuit_2001
  mov R2, [BP-1]
  iadd R2, 3
  mov R1, [R2]
  and R0, R1
__LogicalAnd_ShortCircuit_2001:
  jf R0, __if_1999_end
  mov R0, [BP-2]
  mov R1, [BP-1]
  iadd R1, 1
  mov [R1], R0
__if_2009_start:
  mov R0, [BP-2]
  ine R0, -1
  jf R0, __if_2009_end
  mov R0, [BP-1]
  mov R1, [BP-2]
  mov [R1], R0
__if_2009_end:
  mov R1, [BP-1]
  iadd R1, 2
  mov R0, [R1]
  mov R2, [BP+2]
  iadd R2, 2
  mov R1, [R2]
  iadd R1, 4
  iadd R0, R1
  mov R1, [BP-1]
  iadd R1, 2
  mov [R1], R0
__if_1999_end:
__function_merge_free_malloc_blocks_return:
  mov SP, BP
  pop BP
  ret

__function_reduce_malloc_block:
  push BP
  mov BP, SP
  isub SP, 3
  mov R1, [BP+2]
  iadd R1, 2
  mov R0, [R1]
  mov R1, [BP+3]
  isub R0, R1
  mov [BP-1], R0
__if_2031_start:
  mov R0, [BP-1]
  ile R0, 4
  jf R0, __if_2031_end
  jmp __function_reduce_malloc_block_return
__if_2031_end:
  mov R0, [BP+2]
  iadd R0, 4
  mov R1, [BP+3]
  iadd R0, R1
  mov [BP-2], R0
  mov R0, [BP+2]
  mov R1, [BP-2]
  mov [R1], R0
  mov R1, [BP+2]
  iadd R1, 1
  mov R0, [R1]
  mov R1, [BP-2]
  iadd R1, 1
  mov [R1], R0
  mov R0, 1
  mov R1, [BP-2]
  iadd R1, 3
  mov [R1], R0
  mov R0, [BP-1]
  isub R0, 4
  mov R1, [BP-2]
  iadd R1, 2
  mov [R1], R0
  mov R0, [BP+3]
  mov R1, [BP+2]
  iadd R1, 2
  mov [R1], R0
  mov R0, [BP-2]
  mov R1, [BP+2]
  iadd R1, 1
  mov [R1], R0
__if_2074_start:
  mov R1, [BP-2]
  iadd R1, 1
  mov R0, [R1]
  ine R0, -1
  jf R0, __if_2074_end
  mov R0, [BP-2]
  mov R2, [BP-2]
  iadd R2, 1
  mov R1, [R2]
  mov [R1], R0
__if_2074_end:
  mov R1, [BP-2]
  mov [SP], R1
  call __function_merge_free_malloc_blocks
__function_reduce_malloc_block_return:
  mov SP, BP
  pop BP
  ret

__function_expand_malloc_block:
  push BP
  mov BP, SP
  isub SP, 3
  push R1
  push R2
  isub SP, 2
  mov R0, [BP+3]
  mov R2, [BP+2]
  iadd R2, 2
  mov R1, [R2]
  isub R0, R1
  mov [BP-1], R0
__if_2093_start:
  mov R0, [BP-1]
  ile R0, 0
  jf R0, __if_2093_end
  mov R0, 1
  jmp __function_expand_malloc_block_return
__if_2093_end:
  mov R1, [BP+2]
  iadd R1, 1
  mov R0, [R1]
  mov [BP-2], R0
__if_2103_start:
  mov R0, [BP-2]
  ine R0, -1
  bnot R0
  jt R0, __LogicalOr_ShortCircuit_2106
  mov R2, [BP-2]
  iadd R2, 3
  mov R1, [R2]
  bnot R1
  or R0, R1
__LogicalOr_ShortCircuit_2106:
  jf R0, __if_2103_end
  mov R0, 0
  jmp __function_expand_malloc_block_return
__if_2103_end:
  mov R1, [BP-2]
  iadd R1, 2
  mov R0, [R1]
  iadd R0, 4
  mov [BP-3], R0
__if_2118_start:
  mov R0, [BP-3]
  mov R1, [BP-1]
  ilt R0, R1
  jf R0, __if_2118_end
  mov R0, 0
  jmp __function_expand_malloc_block_return
__if_2118_end:
  mov R1, [BP+2]
  iadd R1, 2
  mov R0, [R1]
  mov R1, [BP-3]
  iadd R0, R1
  mov R1, [BP+2]
  iadd R1, 2
  mov [R1], R0
  mov R1, [BP-2]
  iadd R1, 1
  mov R0, [R1]
  mov R1, [BP+2]
  iadd R1, 1
  mov [R1], R0
__if_2133_start:
  mov R1, [BP-2]
  iadd R1, 1
  mov R0, [R1]
  ine R0, -1
  jf R0, __if_2133_end
  mov R0, [BP+2]
  mov R2, [BP-2]
  iadd R2, 1
  mov R1, [R2]
  mov [R1], R0
__if_2133_end:
  mov R1, [BP+2]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  call __function_reduce_malloc_block
  mov R0, 1
__function_expand_malloc_block_return:
  iadd SP, 2
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_malloc:
  push BP
  mov BP, SP
  isub SP, 3
  push R1
  push R2
__if_2148_start:
  mov R0, [global_malloc_first_block]
  ine R0, -1
  bnot R0
  jf R0, __if_2148_end
  mov R0, [global_malloc_start_address]
  mov [global_malloc_first_block], R0
  mov R0, [global_malloc_end_address]
  mov R1, [global_malloc_start_address]
  isub R0, R1
  iadd R0, 1
  mov R1, [global_malloc_first_block]
  iadd R1, 2
  mov [R1], R0
  mov R1, [global_malloc_first_block]
  iadd R1, 2
  mov R0, [R1]
  isub R0, 4
  mov R1, [global_malloc_first_block]
  iadd R1, 2
  mov [R1], R0
  mov R0, -1
  mov R1, [global_malloc_first_block]
  mov [R1], R0
  mov R0, -1
  mov R1, [global_malloc_first_block]
  iadd R1, 1
  mov [R1], R0
  mov R0, 1
  mov R1, [global_malloc_first_block]
  iadd R1, 3
  mov [R1], R0
__if_2148_end:
__if_2183_start:
  mov R0, [BP+2]
  ile R0, 0
  jf R0, __if_2183_end
  mov R0, -1
  jmp __function_malloc_return
__if_2183_end:
  mov R0, [global_malloc_first_block]
  mov [BP-1], R0
__while_2192_start:
__while_2192_continue:
  mov R0, [BP-1]
  ine R0, -1
  jf R0, __while_2192_end
__if_2195_start:
  mov R1, [BP-1]
  iadd R1, 3
  mov R0, [R1]
  jf R0, __LogicalAnd_ShortCircuit_2198
  mov R2, [BP-1]
  iadd R2, 2
  mov R1, [R2]
  mov R2, [BP+2]
  ige R1, R2
  and R0, R1
__LogicalAnd_ShortCircuit_2198:
  jf R0, __if_2195_end
  jmp __while_2192_end
__if_2195_end:
  mov R1, [BP-1]
  iadd R1, 1
  mov R0, [R1]
  mov [BP-1], R0
  jmp __while_2192_start
__while_2192_end:
__if_2208_start:
  mov R0, [BP-1]
  ine R0, -1
  bnot R0
  jf R0, __if_2208_end
  mov R0, -1
  jmp __function_malloc_return
__if_2208_end:
  mov R0, [BP+2]
  iadd R0, 4
  mov [BP-2], R0
__if_2218_start:
  mov R1, [BP-1]
  iadd R1, 2
  mov R0, [R1]
  mov R1, [BP-2]
  igt R0, R1
  jf R0, __if_2218_else
  mov R0, [BP-1]
  iadd R0, 4
  mov R2, [BP-1]
  iadd R2, 2
  mov R1, [R2]
  iadd R0, R1
  mov R1, [BP-2]
  isub R0, R1
  mov [BP-3], R0
  mov R0, [BP+2]
  mov R1, [BP-3]
  iadd R1, 2
  mov [R1], R0
  mov R0, 0
  mov R1, [BP-3]
  iadd R1, 3
  mov [R1], R0
  mov R0, [BP-1]
  mov R1, [BP-3]
  mov [R1], R0
  mov R1, [BP-1]
  iadd R1, 1
  mov R0, [R1]
  mov R1, [BP-3]
  iadd R1, 1
  mov [R1], R0
  mov R1, [BP-1]
  iadd R1, 2
  mov R0, [R1]
  mov R1, [BP-2]
  isub R0, R1
  mov R1, [BP-1]
  iadd R1, 2
  mov [R1], R0
  mov R0, [BP-3]
  mov R1, [BP-1]
  iadd R1, 1
  mov [R1], R0
__if_2263_start:
  mov R1, [BP-3]
  iadd R1, 1
  mov R0, [R1]
  ine R0, -1
  jf R0, __if_2263_end
  mov R0, [BP-3]
  mov R2, [BP-3]
  iadd R2, 1
  mov R1, [R2]
  mov [R1], R0
__if_2263_end:
  mov R0, [BP-3]
  iadd R0, 4
  jmp __function_malloc_return
  jmp __if_2218_end
__if_2218_else:
  mov R0, 0
  mov R1, [BP-1]
  iadd R1, 3
  mov [R1], R0
  mov R0, [BP-1]
  iadd R0, 4
  jmp __function_malloc_return
__if_2218_end:
__function_malloc_return:
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_free:
  push BP
  mov BP, SP
  isub SP, 2
__if_2288_start:
  mov R0, [BP+2]
  ine R0, -1
  bnot R0
  jf R0, __if_2288_end
  jmp __function_free_return
__if_2288_end:
  mov R0, [BP+2]
  isub R0, 4
  mov [BP-1], R0
  mov R0, 1
  mov R1, [BP-1]
  iadd R1, 3
  mov [R1], R0
  mov R1, [BP-1]
  mov [SP], R1
  call __function_merge_free_malloc_blocks
__function_free_return:
  mov SP, BP
  pop BP
  ret

__function_calloc:
  push BP
  mov BP, SP
  isub SP, 2
  push R1
  isub SP, 3
  mov R0, [BP+2]
  mov R1, [BP+3]
  imul R0, R1
  mov [BP-1], R0
  mov R1, [BP-1]
  mov [SP], R1
  call __function_malloc
  mov [BP-2], R0
__if_2316_start:
  mov R0, [BP-2]
  ine R0, -1
  bnot R0
  jf R0, __if_2316_end
  mov R0, -1
  jmp __function_calloc_return
__if_2316_end:
  mov R1, [BP-2]
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, [BP-1]
  mov [SP+2], R1
  call __function_memset
  mov R0, [BP-2]
__function_calloc_return:
  iadd SP, 3
  pop R1
  mov SP, BP
  pop BP
  ret

__function_realloc:
  push BP
  mov BP, SP
  isub SP, 3
  push R1
  isub SP, 3
__if_2330_start:
  mov R0, [BP+2]
  ine R0, -1
  bnot R0
  jf R0, __if_2330_end
  mov R1, [BP+3]
  mov [SP], R1
  call __function_malloc
  jmp __function_realloc_return
__if_2330_end:
__if_2336_start:
  mov R0, [BP+3]
  ile R0, 0
  jf R0, __if_2336_end
  mov R1, [BP+2]
  mov [SP], R1
  call __function_free
  mov R0, -1
  jmp __function_realloc_return
__if_2336_end:
  mov R0, [BP+2]
  isub R0, 4
  mov [BP-1], R0
  mov R1, [BP-1]
  iadd R1, 2
  mov R0, [R1]
  mov [BP-2], R0
__if_2355_start:
  mov R0, [BP+3]
  mov R1, [BP-2]
  ieq R0, R1
  jf R0, __if_2355_end
  mov R0, [BP+2]
  jmp __function_realloc_return
__if_2355_end:
__if_2361_start:
  mov R0, [BP+3]
  mov R1, [BP-2]
  ilt R0, R1
  jf R0, __if_2361_else
  mov R1, [BP-1]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  call __function_reduce_malloc_block
  mov R0, [BP+2]
  jmp __function_realloc_return
  jmp __if_2361_end
__if_2361_else:
__if_2372_start:
  mov R1, [BP-1]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  call __function_expand_malloc_block
  jf R0, __if_2372_else
  mov R0, [BP+2]
  jmp __function_realloc_return
  jmp __if_2372_end
__if_2372_else:
  mov R1, [BP+3]
  mov [SP], R1
  call __function_malloc
  mov [BP-3], R0
__if_2383_start:
  mov R0, [BP-3]
  ine R0, -1
  bnot R0
  jf R0, __if_2383_end
  mov R0, -1
  jmp __function_realloc_return
__if_2383_end:
  mov R1, [BP-3]
  mov [SP], R1
  mov R1, [BP+2]
  mov [SP+1], R1
  mov R1, [BP-2]
  mov [SP+2], R1
  call __function_memcpy
  mov R1, [BP+2]
  mov [SP], R1
  call __function_free
  mov R0, [BP-3]
  jmp __function_realloc_return
__if_2372_end:
__if_2361_end:
__function_realloc_return:
  iadd SP, 3
  pop R1
  mov SP, BP
  pop BP
  ret

__function_rand:
  push BP
  mov BP, SP
  in R0, RNG_CurrentValue
__function_rand_return:
  mov SP, BP
  pop BP
  ret

__function_srand:
  push BP
  mov BP, SP
  mov R0, [BP+2]
  out RNG_CurrentValue, R0
__function_srand_return:
  mov SP, BP
  pop BP
  ret

__function_exit:
  push BP
  mov BP, SP
  hlt
__function_exit_return:
  mov SP, BP
  pop BP
  ret

__function_get_tile:
  push BP
  mov BP, SP
  push R1
  push R2
__if_2421_start:
  mov R0, [BP+3]
  ilt R0, 0
  jt R0, __LogicalOr_ShortCircuit_2426
  mov R1, [BP+3]
  ige R1, 4
  or R0, R1
__LogicalOr_ShortCircuit_2426:
  jt R0, __LogicalOr_ShortCircuit_2430
  mov R1, [BP+4]
  ilt R1, 0
  or R0, R1
__LogicalOr_ShortCircuit_2430:
  jt R0, __LogicalOr_ShortCircuit_2434
  mov R1, [BP+4]
  ige R1, 3
  or R0, R1
__LogicalOr_ShortCircuit_2434:
  jf R0, __if_2421_end
  mov R0, 0
  jmp __function_get_tile_return
__if_2421_end:
  mov R0, global_island_tiles
  mov R2, [BP+2]
  imul R2, 12
  iadd R0, R2
  mov R2, [BP+3]
  imul R2, 3
  iadd R0, R2
  mov R1, [BP+4]
  iadd R0, R1
  mov R0, [R0]
__function_get_tile_return:
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_generate_island_layout:
  push BP
  mov BP, SP
  isub SP, 7
  mov R0, 2
  mov [BP-1], R0
  mov R0, 1
  mov [BP-2], R0
  call __function_rand
  mov R1, R0
  imod R1, 3
  iadd R1, 2
  mov R0, R1
  mov [BP-3], R0
  mov R0, 0
  mov [BP-4], R0
__for_2468_start:
  mov R0, [BP-4]
  ilt R0, 3
  jf R0, __for_2468_end
  mov R0, 0
  mov [BP-5], R0
__for_2478_start:
  mov R0, [BP-5]
  ilt R0, 4
  jf R0, __for_2478_end
  mov R0, 0
  mov R1, global_island_tiles
  mov R2, [BP+2]
  imul R2, 12
  iadd R1, R2
  mov R2, [BP-5]
  imul R2, 3
  iadd R1, R2
  mov R2, [BP-4]
  iadd R1, R2
  mov [R1], R0
__for_2478_continue:
  mov R0, [BP-5]
  mov R1, R0
  iadd R1, 1
  mov [BP-5], R1
  jmp __for_2478_start
__for_2478_end:
__for_2468_continue:
  mov R0, [BP-4]
  mov R1, R0
  iadd R1, 1
  mov [BP-4], R1
  jmp __for_2468_start
__for_2468_end:
  mov R0, [BP-2]
  mov R1, [BP-3]
  isub R0, R1
  mov [BP-4], R0
__for_2497_start:
  mov R0, [BP-4]
  mov R1, [BP-2]
  mov R2, [BP-3]
  iadd R1, R2
  ile R0, R1
  jf R0, __for_2497_end
  mov R0, [BP-1]
  mov R1, [BP-3]
  isub R0, R1
  mov [BP-5], R0
__for_2511_start:
  mov R0, [BP-5]
  mov R1, [BP-1]
  mov R2, [BP-3]
  iadd R1, R2
  ile R0, R1
  jf R0, __for_2511_end
__if_2525_start:
  mov R0, [BP-5]
  ige R0, 0
  jf R0, __LogicalAnd_ShortCircuit_2530
  mov R1, [BP-5]
  ilt R1, 4
  and R0, R1
__LogicalAnd_ShortCircuit_2530:
  jf R0, __LogicalAnd_ShortCircuit_2534
  mov R1, [BP-4]
  ige R1, 0
  and R0, R1
__LogicalAnd_ShortCircuit_2534:
  jf R0, __LogicalAnd_ShortCircuit_2538
  mov R1, [BP-4]
  ilt R1, 3
  and R0, R1
__LogicalAnd_ShortCircuit_2538:
  jf R0, __if_2525_end
  mov R0, [BP-5]
  mov R1, [BP-1]
  isub R0, R1
  mov [BP-6], R0
  mov R0, [BP-4]
  mov R1, [BP-2]
  isub R0, R1
  mov [BP-7], R0
__if_2552_start:
  mov R1, [BP-6]
  mov R2, [BP-6]
  imul R1, R2
  mov R2, [BP-7]
  mov R3, [BP-7]
  imul R2, R3
  iadd R1, R2
  mov R2, [BP-3]
  mov R3, [BP-3]
  imul R2, R3
  call __function_rand
  mov R3, R0
  imod R3, 2
  iadd R2, R3
  ile R1, R2
  mov R0, R1
  jf R0, __if_2552_end
  mov R0, 1
  mov R1, global_island_tiles
  mov R2, [BP+2]
  imul R2, 12
  iadd R1, R2
  mov R2, [BP-5]
  imul R2, 3
  iadd R1, R2
  mov R2, [BP-4]
  iadd R1, R2
  mov [R1], R0
__if_2552_end:
__if_2525_end:
__for_2511_continue:
  mov R0, [BP-5]
  mov R1, R0
  iadd R1, 1
  mov [BP-5], R1
  jmp __for_2511_start
__for_2511_end:
__for_2497_continue:
  mov R0, [BP-4]
  mov R1, R0
  iadd R1, 1
  mov [BP-4], R1
  jmp __for_2497_start
__for_2497_end:
__function_generate_island_layout_return:
  mov SP, BP
  pop BP
  ret

__function_initialize_islands:
  push BP
  mov BP, SP
  isub SP, 19
  call __function_get_time
  mov R1, R0
  mov [BP-13], R1
  mov R1, [BP-13]
  mov [SP], R1
  call __function_srand
  mov R1, 2
  mov [SP], R1
  call __function_select_texture
  mov R1, 1
  mov [SP], R1
  call __function_select_region
  mov R1, 0
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 64
  mov [SP+2], R1
  mov R1, 64
  mov [SP+3], R1
  mov R1, 32
  mov [SP+4], R1
  mov R1, 32
  mov [SP+5], R1
  call __function_define_region
  mov R0, 10
  mov [global_num_islands], R0
  mov R0, 512.000000
  mov [BP-1], R0
  mov R0, 500.000000
  mov [BP-2], R0
  mov R0, 0
  mov [BP-3], R0
__for_2610_start:
  mov R0, [BP-3]
  mov R1, [global_num_islands]
  ilt R0, R1
  jf R0, __for_2610_end
  mov R0, 0
  mov [BP-4], R0
__while_2626_start:
__while_2626_continue:
  mov R0, [BP-4]
  bnot R0
  jf R0, __while_2626_end
  call __function_rand
  mov R1, R0
  imod R1, 1600
  cif R1
  mov [BP-5], R1
  mov R0, R1
  call __function_rand
  mov R1, R0
  imod R1, 1600
  cif R1
  mov [BP-6], R1
  mov R0, R1
  mov R0, [BP-5]
  fsub R0, 1000.000000
  mov [BP-7], R0
  mov R0, [BP-6]
  fsub R0, 1000.000000
  mov [BP-8], R0
  mov R1, [BP-7]
  mov R2, [BP-7]
  fmul R1, R2
  mov R2, [BP-8]
  mov R3, [BP-8]
  fmul R2, R3
  fadd R1, R2
  mov [SP], R1
  call __function_sqrt
  mov [BP-9], R0
  mov R0, 0
  mov [BP-10], R0
  mov R0, 0
  mov [BP-11], R0
__for_2677_start:
  mov R0, [BP-11]
  mov R1, [BP-3]
  ilt R0, R1
  jf R0, __for_2677_end
  mov R0, [BP-5]
  mov R1, global_island_x
  mov R2, [BP-11]
  iadd R1, R2
  mov R1, [R1]
  fsub R0, R1
  mov [BP-7], R0
  mov R0, [BP-6]
  mov R1, global_island_y
  mov R2, [BP-11]
  iadd R1, R2
  mov R1, [R1]
  fsub R0, R1
  mov [BP-8], R0
  mov R1, [BP-7]
  mov R2, [BP-7]
  fmul R1, R2
  mov R2, [BP-8]
  mov R3, [BP-8]
  fmul R2, R3
  fadd R1, R2
  mov [SP], R1
  call __function_sqrt
  mov [BP-12], R0
__if_2711_start:
  mov R0, [BP-12]
  mov R1, [BP-1]
  flt R0, R1
  jf R0, __if_2711_end
  mov R0, 1
  mov [BP-10], R0
  jmp __for_2677_end
__if_2711_end:
__for_2677_continue:
  mov R0, [BP-11]
  mov R1, R0
  iadd R1, 1
  mov [BP-11], R1
  jmp __for_2677_start
__for_2677_end:
__if_2720_start:
  mov R0, [BP-9]
  mov R1, [BP-2]
  fgt R0, R1
  jf R0, __LogicalAnd_ShortCircuit_2726
  mov R1, [BP-10]
  bnot R1
  and R0, R1
__LogicalAnd_ShortCircuit_2726:
  jf R0, __if_2720_end
  mov R0, 1
  mov [BP-4], R0
  mov R0, [BP-5]
  mov R1, global_island_x
  mov R2, [BP-3]
  iadd R1, R2
  mov [R1], R0
  mov R0, [BP-6]
  mov R1, global_island_y
  mov R2, [BP-3]
  iadd R1, R2
  mov [R1], R0
__if_2720_end:
  jmp __while_2626_start
__while_2626_end:
  mov R1, [BP-3]
  mov [SP], R1
  call __function_generate_island_layout
__for_2610_continue:
  mov R0, [BP-3]
  mov R1, R0
  iadd R1, 1
  mov [BP-3], R1
  jmp __for_2610_start
__for_2610_end:
__function_initialize_islands_return:
  mov SP, BP
  pop BP
  ret

__function_render_islands:
  push BP
  mov BP, SP
  isub SP, 11
  mov R1, 2
  mov [SP], R1
  call __function_select_texture
  mov R0, 0
  mov [BP-1], R0
__for_2748_start:
  mov R0, [BP-1]
  mov R1, [global_num_islands]
  ilt R0, R1
  jf R0, __for_2748_end
  mov R0, global_island_x
  mov R1, [BP-1]
  iadd R0, R1
  mov R0, [R0]
  mov [BP-2], R0
  mov R0, global_island_y
  mov R1, [BP-1]
  iadd R0, R1
  mov R0, [R0]
  mov [BP-3], R0
  mov R0, [BP-2]
  mov R1, [BP+2]
  fsub R0, R1
  mov [BP-4], R0
  mov R0, [BP-3]
  mov R1, [BP+3]
  fsub R0, R1
  mov [BP-5], R0
__if_2778_start:
  mov R0, [BP-4]
  fge R0, -256.000000
  jf R0, __LogicalAnd_ShortCircuit_2786
  mov R1, [BP-4]
  fle R1, 896.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2786:
  jf R0, __LogicalAnd_ShortCircuit_2794
  mov R1, [BP-5]
  fge R1, -192.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2794:
  jf R0, __LogicalAnd_ShortCircuit_2801
  mov R1, [BP-5]
  fle R1, 552.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2801:
  jf R0, __if_2778_end
  mov R0, 0
  mov [BP-6], R0
__for_2809_start:
  mov R0, [BP-6]
  ilt R0, 3
  jf R0, __for_2809_end
  mov R0, 0
  mov [BP-7], R0
__for_2819_start:
  mov R0, [BP-7]
  ilt R0, 4
  jf R0, __for_2819_end
__if_2829_start:
  mov R0, global_island_tiles
  mov R2, [BP-1]
  imul R2, 12
  iadd R0, R2
  mov R2, [BP-7]
  imul R2, 3
  iadd R0, R2
  mov R1, [BP-6]
  iadd R0, R1
  mov R0, [R0]
  ine R0, 0
  jf R0, __if_2829_end
  mov R0, [BP-2]
  mov R1, [BP-7]
  imul R1, 64
  cif R1
  fadd R0, R1
  mov R1, [BP+2]
  fsub R0, R1
  mov [BP-8], R0
  mov R0, [BP-3]
  mov R1, [BP-6]
  imul R1, 64
  cif R1
  fadd R0, R1
  mov R1, [BP+3]
  fsub R0, R1
  mov [BP-9], R0
  mov R1, 1
  mov [SP], R1
  call __function_select_region
  mov R1, [BP-8]
  cfi R1
  mov [SP], R1
  mov R1, [BP-9]
  cfi R1
  mov [SP+1], R1
  call __function_draw_region_at
__if_2829_end:
__for_2819_continue:
  mov R0, [BP-7]
  mov R1, R0
  iadd R1, 1
  mov [BP-7], R1
  jmp __for_2819_start
__for_2819_end:
__for_2809_continue:
  mov R0, [BP-6]
  mov R1, R0
  iadd R1, 1
  mov [BP-6], R1
  jmp __for_2809_start
__for_2809_end:
__if_2778_end:
__for_2748_continue:
  mov R0, [BP-1]
  mov R1, R0
  iadd R1, 1
  mov [BP-1], R1
  jmp __for_2748_start
__for_2748_end:
__function_render_islands_return:
  mov SP, BP
  pop BP
  ret

__function_is_over_island:
  push BP
  mov BP, SP
  isub SP, 5
  push R1
  push R2
  mov R0, 0
  mov [BP-1], R0
__for_2868_start:
  mov R0, [BP-1]
  mov R1, [global_num_islands]
  ilt R0, R1
  jf R0, __for_2868_end
  mov R0, [BP+2]
  mov R1, global_island_x
  mov R2, [BP-1]
  iadd R1, R2
  mov R1, [R1]
  fsub R0, R1
  fdiv R0, 64.000000
  mov [BP-2], R0
  mov R0, [BP+3]
  mov R1, global_island_y
  mov R2, [BP-1]
  iadd R1, R2
  mov R1, [R1]
  fsub R0, R1
  fdiv R0, 64.000000
  mov [BP-3], R0
__if_2898_start:
  mov R0, [BP-2]
  fge R0, 0.000000
  jf R0, __LogicalAnd_ShortCircuit_2903
  mov R1, [BP-2]
  flt R1, 4.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2903:
  jf R0, __LogicalAnd_ShortCircuit_2907
  mov R1, [BP-3]
  fge R1, 0.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2907:
  jf R0, __LogicalAnd_ShortCircuit_2911
  mov R1, [BP-3]
  flt R1, 3.000000
  and R0, R1
__LogicalAnd_ShortCircuit_2911:
  jf R0, __if_2898_end
  mov R0, [BP-2]
  cfi R0
  mov [BP-4], R0
  mov R0, [BP-3]
  cfi R0
  mov [BP-5], R0
__if_2921_start:
  mov R0, global_island_tiles
  mov R2, [BP-1]
  imul R2, 12
  iadd R0, R2
  mov R2, [BP-4]
  imul R2, 3
  iadd R0, R2
  mov R1, [BP-5]
  iadd R0, R1
  mov R0, [R0]
  ine R0, 0
  jf R0, __if_2921_end
  mov R0, 1
  jmp __function_is_over_island_return
__if_2921_end:
__if_2898_end:
__for_2868_continue:
  mov R0, [BP-1]
  mov R1, R0
  iadd R1, 1
  mov [BP-1], R1
  jmp __for_2868_start
__for_2868_end:
  mov R0, 0
__function_is_over_island_return:
  pop R2
  pop R1
  mov SP, BP
  pop BP
  ret

__function_initialize_carrier:
  push BP
  mov BP, SP
  isub SP, 6
  mov R1, 1
  mov [SP], R1
  call __function_select_texture
  mov R1, 0
  mov [SP], R1
  call __function_select_region
  mov R1, 0
  mov [SP], R1
  mov R1, 0
  mov [SP+1], R1
  mov R1, 77
  mov [SP+2], R1
  mov R1, 255
  mov [SP+3], R1
  mov R1, 38
  mov [SP+4], R1
  mov R1, 127
  mov [SP+5], R1
  call __function_define_region
__function_initialize_carrier_return:
  mov SP, BP
  pop BP
  ret

__function_render_world:
  push BP
  mov BP, SP
  isub SP, 2
  mov R1, -8368096
  mov [SP], R1
  call __function_clear_screen
  mov R1, 1
  mov [SP], R1
  call __function_select_texture
  mov R1, 0
  mov [SP], R1
  call __function_select_region
  mov R1, [BP+2]
  fsgn R1
  fadd R1, 1000.000000
  cfi R1
  mov [SP], R1
  mov R1, [BP+3]
  fsgn R1
  fadd R1, 1000.000000
  cfi R1
  mov [SP+1], R1
  call __function_draw_region_at
  mov R1, [BP+2]
  mov [SP], R1
  mov R1, [BP+3]
  mov [SP+1], R1
  call __function_render_islands
__function_render_world_return:
  mov SP, BP
  pop BP
  ret

__function_update_menu:
  push BP
  mov BP, SP
__if_2993_start:
  call __function_gamepad_button_start
  mov R1, R0
  ieq R1, 1
  mov R0, R1
  jf R0, __if_2993_end
  mov R0, 1
  mov [global_game_state], R0
__if_2993_end:
__function_update_menu_return:
  mov SP, BP
  pop BP
  ret

__function_render_menu:
  push BP
  mov BP, SP
  isub SP, 3
  mov R1, -16777216
  mov [SP], R1
  call __function_clear_screen
  mov R1, -1
  mov [SP], R1
  call __function_select_texture
  mov R1, -1
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, 260
  mov [SP], R1
  mov R1, 140
  mov [SP+1], R1
  mov R1, __literal_string_3022
  mov [SP+2], R1
  call __function_print_at
__if_3023_start:
  call __function_get_frame_counter
  mov R1, R0
  idiv R1, 30
  imod R1, 2
  ieq R1, 0
  mov R0, R1
  jf R0, __if_3023_end
  mov R1, 230
  mov [SP], R1
  mov R1, 200
  mov [SP+1], R1
  mov R1, __literal_string_3046
  mov [SP+2], R1
  call __function_print_at
__if_3023_end:
__function_render_menu_return:
  mov SP, BP
  pop BP
  ret

__function_clamp:
  push BP
  mov BP, SP
  push R1
__if_3051_start:
  mov R0, [BP+2]
  mov R1, [BP+3]
  flt R0, R1
  jf R0, __if_3051_end
  mov R0, [BP+3]
  jmp __function_clamp_return
__if_3051_end:
__if_3057_start:
  mov R0, [BP+2]
  mov R1, [BP+4]
  fgt R0, R1
  jf R0, __if_3057_end
  mov R0, [BP+4]
  jmp __function_clamp_return
__if_3057_end:
  mov R0, [BP+2]
__function_clamp_return:
  pop R1
  mov SP, BP
  pop BP
  ret

__function_update_gameover:
  push BP
  mov BP, SP
__if_3068_start:
  call __function_gamepad_button_start
  mov R1, R0
  ieq R1, 1
  mov R0, R1
  jf R0, __if_3068_end
  call __function_reset_airplane
  mov R0, 0
  mov [global_game_state], R0
__if_3068_end:
__function_update_gameover_return:
  mov SP, BP
  pop BP
  ret

__function_render_gameover:
  push BP
  mov BP, SP
  isub SP, 3
  mov R1, -16777216
  mov [SP], R1
  call __function_clear_screen
  mov R1, -1
  mov [SP], R1
  call __function_select_texture
  mov R1, -16776961
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, 275
  mov [SP], R1
  mov R1, 160
  mov [SP+1], R1
  mov R1, __literal_string_3098
  mov [SP+2], R1
  call __function_print_at
__if_3099_start:
  call __function_get_frame_counter
  mov R1, R0
  idiv R1, 30
  imod R1, 2
  ieq R1, 0
  mov R0, R1
  jf R0, __if_3099_end
  mov R1, -1
  mov [SP], R1
  call __function_set_multiply_color
  mov R1, 230
  mov [SP], R1
  mov R1, 200
  mov [SP+1], R1
  mov R1, __literal_string_3124
  mov [SP+2], R1
  call __function_print_at
__if_3099_end:
__function_render_gameover_return:
  mov SP, BP
  pop BP
  ret

__function_main:
  push BP
  mov BP, SP
  call __function_initialize_carrier
  call __function_initialize_islands
  call __function_initialize_airplane
  mov R0, 0
  mov [global_game_state], R0
__while_3134_start:
__while_3134_continue:
  mov R0, 1
  jf R0, __while_3134_end
  mov R0, [global_game_state]
  mov R1, 0
  ieq R1, R0
  jt R1, __switch_3137_case_0
  mov R1, 1
  ieq R1, R0
  jt R1, __switch_3137_case_1
  mov R1, 2
  ieq R1, R0
  jt R1, __switch_3137_case_2
  jmp __switch_3137_end
__switch_3137_case_0:
  call __function_update_menu
  call __function_render_menu
  jmp __switch_3137_end
__switch_3137_case_1:
  call __function_update_airplane
  call __function_render_airplane
  jmp __switch_3137_end
__switch_3137_case_2:
  call __function_update_gameover
  call __function_render_gameover
__switch_3137_end:
  call __function_end_frame
  jmp __while_3134_start
__while_3134_end:
__function_main_return:
  mov SP, BP
  pop BP
  ret

__literal_string_1261:
  string "0123456789ABCDEF"
__literal_string_1371:
  string "."
__literal_string_1382:
  string "0"
__literal_string_1491:
  string "FUEL:"
__literal_string_3022:
  string "OCEAN STORM"
__literal_string_3046:
  string "PRESS START TO BEGIN"
__literal_string_3098:
  string "GAME OVER"
__literal_string_3124:
  string "PRESS START TO CONTINUE"
