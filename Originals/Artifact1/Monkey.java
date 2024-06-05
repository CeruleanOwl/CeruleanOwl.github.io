/*
 * Author: Wesley Roberts
 * Date: 4/4/2021
*/
package Graz;

public class Monkey extends RescueAnimal {
	
	//Attributes
	private String tailLength;
	private String height;
	private String bodyLength;
	private String species;
	
	//constructor
    public Monkey(String name, String species, String gender, String age,
    String weight, String acquisitionDate, String acquisitionCountry,
	String trainingStatus, boolean reserved, String inServiceCountry, String height, String bodyLength, String tailLength) {
        setName(name);
        setSpecies(species);
        setGender(gender);
        setAge(age);
        setWeight(weight);
        setAcquisitionDate(acquisitionDate);
        setAcquisitionLocation(acquisitionCountry);
        setTrainingStatus(trainingStatus);
        setReserved(reserved);
        setInServiceCountry(inServiceCountry);
        setHeight(height);
        setTailLength(tailLength);
        setBodyLength(bodyLength);
    }
        
    //Setter Method 
    public String getSpecies() {
        return species;
    }

    // Mutator Method
    public void setSpecies(String species) {
        this.species = species;
    }
    
    //Setter Method
    public String getTailLength() {
        return tailLength;
    }

    // Mutator Method
    public void setTailLength(String tailLength) {
        this.tailLength = tailLength;
    }
    
    //Setter Method
    public String getHeight() {
        return height;
    }

    // Mutator Method
    public void setHeight(String height) {
        this.height = height;
    }
    
    //Setter Method
    public String getBodyLength() {
        return bodyLength;
    }

    // Mutator Method
    public void setBodyLength(String bodyLength) {
        this.bodyLength = bodyLength;
    }
}
