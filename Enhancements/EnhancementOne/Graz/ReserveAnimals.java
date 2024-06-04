/*
 * Author: Wesley Roberts
 * Date: 5/19/2024
*/
package Graz;
import java.util.Scanner;
import java.util.ArrayList;

public class ReserveAnimals {
    public static void reserveAnimal(Scanner scanner, ArrayList<Dog> dogList, ArrayList<Monkey> monkeyList, ArrayList<Cat> catList) {
            System.out.println("What is the animal's name?");
            String name = scanner.nextLine();
            String type0 = "";
            
            while (true) {
	            System.out.println("What is the type of animal? (Monkey, Dog, Cat)");
	            String type = scanner.nextLine();
	            if (type.equalsIgnoreCase("Monkey") || (type.equalsIgnoreCase("Dog")) || (type.equalsIgnoreCase("Cat"))) {
	            	type0 = type;
	            	break;
	            }
	            else {
	            	System.out.println("Not a type.");
	            }
            }
            
            System.out.println("What is the service country of the animal?");
            String inServiceCountry = scanner.nextLine();
            
            if (type0.equalsIgnoreCase("monkey")) {
            	int monkeyChecker = 0; //Checks if the information never passed conditions.
            	for (Monkey monkey : monkeyList) {
            		if (monkey.getName().equalsIgnoreCase(name) && (monkey.getInServiceLocation().equalsIgnoreCase(inServiceCountry)))  {
            			monkey.setReserved(true);
                        System.out.println("╔════════════════════════════════════════════════╗\n" +
                                           "║Monkey was found and Reserved!                  ║\n" +
                                           "╚════════════════════════════════════════════════╝");
            			monkeyChecker++;
            		}
            	}
        		if (monkeyChecker == 0) {
        			System.out.println("Could not find monkey by that name and service country.");
        		}
            }
            
            else if (type0.equalsIgnoreCase("dog")) {
            	int dogChecker = 0;
            	for (Dog dog : dogList) {
            		if (dog.getName().equalsIgnoreCase(name) && (dog.getInServiceLocation().equalsIgnoreCase(inServiceCountry)))  {
            			dog.setReserved(true);
                        System.out.println("╔════════════════════════════════════════════════╗\n" +
                                           "║Dog was found and Reserved!                     ║\n" +
                                           "╚════════════════════════════════════════════════╝");
            			dogChecker++;
            		}
            		if (dogChecker == 0) {
            			System.out.println("Could not find dog by that name and service country.");
            		}
            	}
            }

			else if (type0.equalsIgnoreCase("cat")) {
            	int catChecker = 0;
            	for (Cat cat : catList) {
            		if (cat.getName().equalsIgnoreCase(name) && (cat.getInServiceLocation().equalsIgnoreCase(inServiceCountry)))  {
            			cat.setReserved(true);
                        System.out.println("╔════════════════════════════════════════════════╗\n" +
                                           "║Cat was found and Reserved!                     ║\n" +
                                           "╚════════════════════════════════════════════════╝");
            			catChecker++;
            		}
            		if (catChecker == 0) {
            			System.out.println("Could not find cat by that name and service country.");
            		}
            	}
            }
            	
            

        }
}
