package Assignment

import Assignment.CSV_writer
import org.apache.spark.SparkConf

import org.apache.spark.sql.functions.{ window, column, desc, col }

import org.apache.spark.sql.SQLContext
import org.apache.spark.sql.Column
import org.apache.spark.sql.Row
import org.apache.spark.sql.types.StructType
import org.apache.spark.sql.types.{ ArrayType, StringType, StructField, IntegerType, DoubleType }
import org.apache.spark.sql.types.DataType
import org.apache.spark.sql.functions.col
import org.apache.spark.sql.functions.{ count, sum, min, max, asc, desc, udf, to_date, avg }
import org.apache.spark.sql.functions.unix_timestamp
import org.apache.spark.sql.functions._

import org.apache.spark.sql.functions.explode
import org.apache.spark.sql.functions.array

import org.apache.spark.sql.SparkSession
import org.apache.spark.SparkContext


import java.lang.Thread
import org.apache.spark.rdd.RDD

import scala.math.Pi
import scala.math.sin
import scala.math.cos
import scala.math.atan
import scala.math.pow
import scala.math.sqrt
import scala.math.min


import org.apache.log4j.Logger
import org.apache.log4j.Level

case class Accident(id: String,
                    X: Double,
                    Y: Double,
                    Xt: Double,
                    Yt: Double)
                    
object RDD_operations extends App {

  def k_means_clustering_rdd_3d(lines: RDD[String]){
    
    //deleting header
    val withoutHeader = lines.mapPartitionsWithIndex{ (index, iter) => if (index == 0) iter.drop(1) else iter}
    
    //extracting id, x, y and dayofweek
    val xyDOW = withoutHeader.map(line => {
      val lines = line.split(";")
      //x is at index 55, y is at index 56 and dayofweek is at index 19
      (lines(0), lines(55), lines(56), DOW_to_int(lines(19).toLowerCase))
    })
    //clean the data from empty cells (if weekday has been informal it has been changed to 0)
    val clean_xyDOW = xyDOW.filter(line => line._1 != "" && 
                                    line._2 != "" && line._3 != "" && line._4 != 0)
    //projecting the data to unitcirle
    val projected_xyDOW = clean_xyDOW.map(line => {
      Accident(id=line._1, X=line._2.toDouble, Y=line._3.toDouble, Xt=DOW_to_Xt(line._4),
                Yt=DOW_to_Yt(line._4))
    })
    
    //clustercount
    val k = 30
    //taking random means for the first iteration
    val initialMeans = projected_xyDOW.takeSample(false, k, System.nanoTime.toInt).map(a => (a.X, a.Y, a.Xt, a.Yt)) 
   
    val means = kmeans(initialMeans, projected_xyDOW)

    //value 1 is placer to centersArr to declare this array
    val centersArr: Array[Array[Double]] = means.map(arr => {
      Xt_Yt_to_DOW(arr)
   })
   
   val headers = Array("X", "Y")
   //Write the result to cvs
   CSV_writer.writeCsv("task5", headers, centersArr)
 }
  
  /**
   * Recursive function that counts the cluster means until they
   * don't move anymore or iterations grow to the maximum amount
   */
  def kmeans(means: Array[(Double, Double, Double, Double)],
              accRDD: RDD[Accident],
              iter: Int = 1) : Array[(Double, Double, Double, Double)] = {
    
    //max iterations = 50
    if (iter == 50) means
    else {
      
      // First map values to form (centroid, Photo). Then group the values by key
      val centroid_means = accRDD.map(p=>(find_closest((p.X,p.Y,p.Xt,p.Yt), means),p))
                                  .groupByKey()
      
      // Count the new means from iterables and change it to Array
      val new_means = centroid_means.mapValues(p=>averageAcc(p)).values.collect
      
      // If the centroids didn't move, they're in their final places.
      if (new_means == means) means
      // Otherwise call the function again
      else kmeans(new_means, accRDD, iter+1)  
    }
                                
  }

  /**
   * Transforms weekday string to int (1-7)
   * returns the int value for weekday
   */
  def DOW_to_int(DOW: String) : Int = {
    DOW match{
      case "maanantai" => return 1
      case "tiistai" => return 2
      case "keskiviikko" => return 3
      case "torstai" => return 4
      case "perjantai" => return 5
      case "lauantai" => return 6
      case "sunnuntai" => return 7
      case _ => return 0
    }
  }
  /**
 	* porjects DOW to unitcircle in respect of X
 	*/
  def DOW_to_Xt(DOW: Int): Double = {
    return sin(2*Pi*DOW.toFloat/7)
  }
  
  /**
   * porjects DOW to unitcircle in respect of Y
   */
  def DOW_to_Yt(DOW: Int): Double = {
    return cos(2*Pi*DOW.toFloat/7)
  }
  
  /**
   * Calculates euclidean distance in 4-dimensional space
   * returns the distance
   */
  def distance(p: (Double, Double, Double, Double), 
      q: (Double, Double, Double, Double)): Double = {
    
    val dist = sqrt( pow(p._1-q._1,2) + pow(p._2-q._2,2) + pow(p._3-q._3,2) + pow(p._4-q._4,2) )
    return dist
  }
  
  /**
   * Finds the closest mean point from the means array
   * returns the index of the closest mean
   */
  def find_closest(p: (Double, Double, Double, Double), 
      means: Array[(Double, Double, Double, Double)]) : Int = {

   val distances = means.map(m => {
     distance(p, m)
   })
   val i_closest = distances.indexOf(distances.min)
   return i_closest
  }
  
  /**
   * calculates the average point in 4 dimensional space for accidents
   */
  def averageAcc(i: Iterable[Accident]) : (Double, Double, Double, Double) = {
    var xavg = 0.0
    var yavg = 0.0
    var xtavg = 0.0
    var ytavg = 0.0
    for (accident<-i) {
      xavg += accident.X 
      yavg += accident.Y
      xtavg += accident.Xt
      ytavg += accident.Yt
    }
    xavg = xavg/i.size
    yavg = yavg/i.size
    xtavg = xtavg/i.size
    ytavg = ytavg/i.size
    
    return (xavg, yavg, xtavg, ytavg)
  }
  
  /**
   * transforms the X,Y,Xt,Yt data back to 3 dimensional X,Y,DOW data
   * returns an array which contains this data
   */
  def Xt_Yt_to_DOW(center: (Double, Double, Double, Double)): Array[Double] = {
        
    //take the Xt and Yt values from array
    val Xt = center._3
    val Yt = center._4
    //calculate the day
    var day = atan(Yt/Xt)/2/Pi*7
    
    val x = sin(2*Pi*(Xt/7))
    val y = cos(2*Pi*(Yt/7))
    //if the new x and y dont match with the previous, the point is on the left side of the unitcirle
    // in this case, Pi is added so the angle is correctly calculated
    if (x != Xt || y != Yt) day = (atan(Yt/Xt)+Pi)/2/Pi*7 else day = day
    //for negative angles
    if (day < 0) day += 7 else day = day
    
    val center3d = Array(center._1, center._2, day)
    
    return center3d
  }
}