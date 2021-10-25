#include "fun.h"


int main(void)
{
  // System inputs                                      System_Inputs_leftSensor -> System_Inputs_centerSensor -> System_Inputs_rightSensor
  struct io_type *System_Inputs_leftSensor   = NULL;
  struct io_type *System_Inputs_centerSensor = NULL;
  struct io_type *System_Inputs_rightSensor  = NULL;

  // System outputs
  struct io_type *System_Outputs = NULL;

  // Funciones de pertenencias
  struct mf_type *membership_functions_inputs_cerca_right  = NULL;  
  struct mf_type *membership_functions_inputs_lejos_right  = NULL;  
  struct mf_type *membership_functions_inputs_cerca_center = NULL;  
  struct mf_type *membership_functions_inputs_lejos_center = NULL; 
  struct mf_type *membership_functions_inputs_cerca_left   = NULL;  
  struct mf_type *membership_functions_inputs_lejos_left   = NULL; 

  struct mf_type *membership_functions_outputs_goLeft      = NULL;
  struct mf_type *membership_functions_outputs_goRight     = NULL;
  struct mf_type *membership_functions_outputs_goCenter    = NULL;


  long long int rightSensor, centerSensor, leftSensor;

  //////////////////////////////////////////////// Reglas
  //////////////////////////////////////////////// leftSensor, centerSensor, rightSensor
  struct rule_type         *rule_0           = NULL;                  
  struct rule_element_type *if_side_0_left   = NULL;
  struct rule_element_type *if_side_0_right  = NULL;
  struct rule_element_type *if_side_0_center = NULL;
  struct rule_element_type *then_side_0      = NULL; // Si CERCA, CERCA , CERCA = CENTRO
  struct rule_type         *rule_1           = NULL;
  struct rule_element_type *if_side_1_left   = NULL;
  struct rule_element_type *if_side_1_right  = NULL; 
  struct rule_element_type *if_side_1_center = NULL;
  struct rule_element_type *then_side_1      = NULL; // Si CERCA, CERCA , LEJOS = DERECHA

  struct rule_type         *rule_2           = NULL;
  struct rule_element_type *if_side_2_left   = NULL;
  struct rule_element_type *if_side_2_right  = NULL; 
  struct rule_element_type *if_side_2_center = NULL;
  struct rule_element_type *then_side_2      = NULL; // Si CERCA, LEJOS , CERCA = CENTRO

  struct rule_type         *rule_3           = NULL;
  struct rule_element_type *if_side_3_left   = NULL;
  struct rule_element_type *if_side_3_right  = NULL; 
  struct rule_element_type *if_side_3_center = NULL;
  struct rule_element_type *then_side_3      = NULL; // Si CERCA, LEJOS , LEJOS = DERECHA

  struct rule_type         *rule_4           = NULL;
  struct rule_element_type *if_side_4_left   = NULL;
  struct rule_element_type *if_side_4_right  = NULL; 
  struct rule_element_type *if_side_4_center = NULL;
  struct rule_element_type *then_side_4      = NULL; // Si LEJOS, CERCA , CERCA = IZQUIERDA

  struct rule_type         *rule_5           = NULL;
  struct rule_element_type *if_side_5_left   = NULL;
  struct rule_element_type *if_side_5_right  = NULL; 
  struct rule_element_type *if_side_5_center = NULL;
  struct rule_element_type *then_side_5      = NULL; // Si LEJOS, CERCA , LEJOS = DERECHA

  struct rule_type         *rule_6           = NULL;
  struct rule_element_type *if_side_6_left   = NULL;
  struct rule_element_type *if_side_6_right  = NULL; 
  struct rule_element_type *if_side_6_center = NULL;
  struct rule_element_type *then_side_6      = NULL; // Si LEJOS, LEJOS , CERCA = IZQUIERDA

  struct rule_type         *rule_7           = NULL;
  struct rule_element_type *if_side_7_left   = NULL;
  struct rule_element_type *if_side_7_right  = NULL; 
  struct rule_element_type *if_side_7_center = NULL;
  struct rule_element_type *then_side_7      = NULL; // Si LEJOS, LEJOS , LEJOS = CENTRO

  printf("Entradas: leftSensor, centerSensor, rightSensor \n");

    //while(1){

      // Leo las entradas del archivo "state.txt"
      get_system_inputs(&rightSensor, &centerSensor, &leftSensor);

      // Cargo las funciones de pertenencias  TENGO QUE TRIPLICAR ESTO!!!
      // Funciones de pertenencia para rightSensor
      membership_functions_inputs_lejos_right = initialize_membership_inputs("Lejos", 0,  30, 970, (float)1/30, (float)1/30, NULL);
      membership_functions_inputs_cerca_right = initialize_membership_inputs("Cerca", 0, -60, 60, (float)1/30, (float)1/30, membership_functions_inputs_lejos_right);

      // Funciones de pertenencia para centerSensor
      membership_functions_inputs_lejos_center = initialize_membership_inputs("Lejos", 0,  30,  970, (float)1/30, (float)1/30, NULL);
      membership_functions_inputs_cerca_center = initialize_membership_inputs("Cerca", 0, -60,  60, (float)1/30, (float)1/30, membership_functions_inputs_lejos_center);

      // Funciones de pertenencia para leftSensor
      membership_functions_inputs_lejos_left = initialize_membership_inputs("Lejos", 0,  30,  970, (float)1/30, (float)1/30, NULL);
      membership_functions_inputs_cerca_left = initialize_membership_inputs("Cerca", 0, -60,  60, (float)1/30, (float)1/30, membership_functions_inputs_lejos_left);

      // Cargo las estructuras de las entradas 
      System_Inputs_rightSensor  = initialize_system_io("Right", rightSensor, membership_functions_inputs_cerca_right, NULL);
      if (System_Inputs_rightSensor == NULL)
        return -1;
      System_Inputs_centerSensor = initialize_system_io("Center", centerSensor, membership_functions_inputs_cerca_center, System_Inputs_rightSensor);
      if (System_Inputs_centerSensor == NULL)
        return -1;
      System_Inputs_leftSensor   = initialize_system_io("Left", leftSensor, membership_functions_inputs_cerca_left, System_Inputs_centerSensor);
      if (System_Inputs_leftSensor == NULL)
        return -1;

      fuzzification(System_Inputs_leftSensor);
      
      // Si LEJOS, LEJOS , LEJOS = CENTRO
      if_side_7_right  = add_rule_element(if_side_7_right,  System_Inputs_rightSensor ->membership_functions->next->value, NULL);
      if_side_7_center = add_rule_element(if_side_7_center, System_Inputs_centerSensor->membership_functions->next->value, if_side_7_right);
      if_side_7_left   = add_rule_element(if_side_7_left,   System_Inputs_leftSensor  ->membership_functions->next->value, if_side_7_center);
      then_side_7      = add_rule_element(then_side_7,      0.0, NULL);
      rule_7           = add_rule(rule_7, "LEJOS, LEJOS , LEJOS = CENTRO", if_side_7_left, then_side_7, NULL);

      // Si LEJOS, LEJOS , CERCA = IZQUIERDA
      if_side_6_right  = add_rule_element(if_side_6_right,  System_Inputs_rightSensor ->membership_functions->value, NULL);
      if_side_6_center = add_rule_element(if_side_6_center, System_Inputs_centerSensor->membership_functions->next->value, if_side_6_right);
      if_side_6_left   = add_rule_element(if_side_6_left,   System_Inputs_leftSensor  ->membership_functions->next->value, if_side_6_center);
      then_side_6      = add_rule_element(then_side_6,      0.0, NULL);
      rule_6           = add_rule(rule_6, "LEJOS, LEJOS , CERCA = IZQUIERDA", if_side_6_left, then_side_6, rule_7);

      // Si LEJOS, CERCA , LEJOS = DERECHA
      if_side_5_right  = add_rule_element(if_side_5_right,  System_Inputs_rightSensor ->membership_functions->next->value, NULL);
      if_side_5_center = add_rule_element(if_side_5_center, System_Inputs_centerSensor->membership_functions->value, if_side_5_right);
      if_side_5_left   = add_rule_element(if_side_5_left,   System_Inputs_leftSensor  ->membership_functions->next->value, if_side_5_center);
      then_side_5      = add_rule_element(then_side_5,      0.0, NULL);
      rule_5           = add_rule(rule_5, "LEJOS, CERCA , LEJOS = DERECHA", if_side_5_left, then_side_5, rule_6);

      // Si LEJOS, CERCA , CERCA = IZQUIERDA
      if_side_4_right  = add_rule_element(if_side_4_right,  System_Inputs_rightSensor ->membership_functions->value, NULL);
      if_side_4_center = add_rule_element(if_side_4_center, System_Inputs_centerSensor->membership_functions->value, if_side_4_right);
      if_side_4_left   = add_rule_element(if_side_4_left,   System_Inputs_leftSensor  ->membership_functions->next->value, if_side_4_center);
      then_side_4      = add_rule_element(then_side_4,      0.0, NULL);
      rule_4           = add_rule(rule_4, "LEJOS, CERCA , CERCA = IZQUIERDA", if_side_4_left, then_side_4, rule_5);

      // Si CERCA, LEJOS , LEJOS = DERECHA
      if_side_3_right  = add_rule_element(if_side_3_right,  System_Inputs_rightSensor ->membership_functions->next->value, NULL);
      if_side_3_center = add_rule_element(if_side_3_center, System_Inputs_centerSensor->membership_functions->next->value, if_side_3_right);
      if_side_3_left   = add_rule_element(if_side_3_left,   System_Inputs_leftSensor  ->membership_functions->value, if_side_3_center);
      then_side_3      = add_rule_element(then_side_3,      0.0, NULL);
      rule_3           = add_rule(rule_3, "CERCA, LEJOS , LEJOS = DERECHA", if_side_3_left, then_side_3, rule_4);
      
      // Si CERCA, LEJOS , CERCA = CENTRO
      if_side_2_right  = add_rule_element(if_side_2_right,  System_Inputs_rightSensor ->membership_functions->value, NULL);
      if_side_2_center = add_rule_element(if_side_2_center, System_Inputs_centerSensor->membership_functions->next->value, if_side_2_right);
      if_side_2_left   = add_rule_element(if_side_2_left,   System_Inputs_leftSensor  ->membership_functions->value, if_side_2_center);
      then_side_2      = add_rule_element(then_side_2,      0.0, NULL);
      rule_2           = add_rule(rule_2, "CERCA, LEJOS , CERCA = CENTRO", if_side_2_left, then_side_2, rule_3);
      
      // Si CERCA, CERCA , LEJOS = DERECHA
      if_side_1_right  = add_rule_element(if_side_1_right,  System_Inputs_rightSensor ->membership_functions->next->value, NULL);
      if_side_1_center = add_rule_element(if_side_1_center, System_Inputs_centerSensor->membership_functions->value, if_side_1_right);
      if_side_1_left   = add_rule_element(if_side_1_left,   System_Inputs_leftSensor  ->membership_functions->value, if_side_1_center);
      then_side_1      = add_rule_element(then_side_1,      0.0, NULL);
      rule_1           = add_rule(rule_1, "CERCA, CERCA , LEJOS = DERECHA", if_side_1_left, then_side_1, rule_2);
      
      //printf("Test: %f %f %f \n",  System_Inputs_leftSensor->membership_functions->value, System_Inputs_centerSensor->membership_functions->next->value, System_Inputs_rightSensor->membership_functions->value);
      // Si CERCA, CERCA , CERCA = CENTRO
      if_side_0_right  = add_rule_element(if_side_0_right,  System_Inputs_rightSensor ->membership_functions->value, NULL);
      if_side_0_center = add_rule_element(if_side_0_center, System_Inputs_centerSensor->membership_functions->value, if_side_0_right);
      if_side_0_left   = add_rule_element(if_side_0_left,   System_Inputs_leftSensor  ->membership_functions->value, if_side_0_center);
      then_side_0      = add_rule_element(then_side_0,      0.0, NULL);
      rule_0           = add_rule(rule_0, "CERCA, CERCA , CERCA = CENTRO", if_side_0_left, then_side_0, rule_1);

      // Output
      membership_functions_outputs_goRight  = initialize_membership_inputs("goRight",  0,    5, 355, (float)1/5, (float)1/5, NULL);
      membership_functions_outputs_goCenter = initialize_membership_inputs("goCenter", 0,  -10,  10, (float)1/5, (float)1/5, membership_functions_outputs_goRight);
      membership_functions_outputs_goLeft   = initialize_membership_inputs("goLeft",   0, -355,  -5, (float)1/5, (float)1/5, membership_functions_outputs_goCenter);

      System_Outputs   = initialize_system_io("Angulo", 0, membership_functions_outputs_goLeft, NULL);
      if (System_Outputs == NULL)
        return -1;


      rule_evaluation(rule_0, System_Outputs);
      defuzzification(System_Outputs);
      
    //     put_system_outputs();
    // }

  return 0;
}