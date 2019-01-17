package Assignment

import org.apache.spark.SparkConf

import org.apache.spark.sql.functions.{ window, column, desc, col }

import org.apache.spark.ml.clustering.KMeans
import org.apache.spark.ml.feature.VectorAssembler
import org.apache.spark.ml.Pipeline
import org.apache.spark.ml.linalg.DenseVector


import org.apache.spark.sql.SQLContext
import org.apache.spark.sql.Column
import org.apache.spark.sql.Row
import org.apache.spark.sql.DataFrame
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

import com.databricks.spark.xml._
import org.apache.spark.storage.StorageLevel
import org.apache.spark.streaming.{ Seconds, StreamingContext }

import java.lang.Thread
import org.apache.spark.rdd.RDD

import org.apache.log4j.Logger
import org.apache.log4j.Level

import scala.math.Pi
import scala.math.sin
import scala.math.cos
import scala.math.atan
import scala.math.abs


import Assignment.CSV_writer
import org.apache.spark.ml.clustering.KMeansModel


object DataFrame_operations extends App {
    
    /**
     * Clusters the location data (X,Y) and send the cluster centers to CSV writer
     */
    def k_means_clustering_dataframe_2d(data: DataFrame){
      
      //Choosing location data and cleaning it from nulls.
      val xy_data: DataFrame = data.select("X", "Y")
      val clean_xy_data = clean_nullsXY(xy_data)
  
      //K-means clustering
      val vectorAssembler = new VectorAssembler()
                                .setInputCols(Array("X", "Y"))
                                .setOutputCol("features")
      val transformationPipeline = new Pipeline().setStages(Array(vectorAssembler))
      val pipeLine = transformationPipeline.fit(clean_xy_data)
      val transformedTraining = pipeLine.transform(clean_xy_data)
      val clustercount = 300
      
      val kmModel = fit_kmModel(clustercount, transformedTraining)
      
      //kmModel is Array[Vector[Double]] --> needs to be transformed to Array[Array[Double]]
      val centersArr = kmModel.clusterCenters.map(center => center.toArray)
      val headers = Array("X", "Y")
  
      CSV_writer.writeCsv("basic", headers, centersArr)
   
    }
    
  def fit_kmModel(clustercount: Int, transformedTraining: DataFrame) : KMeansModel = {
  
      val kmeans = new KMeans().setK(clustercount).setSeed(1L) 
      val kmModel = kmeans.fit(transformedTraining)
      return kmModel
       
  }
  
  def count_elbow_values_2d(data: DataFrame) {
         //Choosing location data and cleaning it from nulls.
      val xy_data: DataFrame = data.select("X", "Y")
      val clean_xy_data = clean_nullsXY(xy_data)
  
      //K-means clustering
      val vectorAssembler = new VectorAssembler()
                                .setInputCols(Array("X", "Y"))
                                .setOutputCol("features")
      val transformationPipeline = new Pipeline().setStages(Array(vectorAssembler))
      val pipeLine = transformationPipeline.fit(clean_xy_data)
      val transformedTraining = pipeLine.transform(clean_xy_data)
      val clustercount = 20
      for (clustercount <- 20 to 50) {
        
        val kmModel = fit_kmModel(clustercount, transformedTraining)
        val WSSSE = kmModel.computeCost(transformedTraining)
        println(clustercount + "," + WSSSE)
        
      }
  }
  
  def count_elbow_values_3d(data: DataFrame) {
    val xyDOW_data: DataFrame = data.select("X", "Y", "Vkpv")
    
    //udf to change day of week from "Maanantai, Tiistai,..." to 1-7
    val DOW_to_int_conversion = udf {(Vkpv: String) =>
      val DOW_int = DOW_to_int(Vkpv.toLowerCase)
      DOW_int
    }
    //udf to project the DOW value on unitcircle in respect of X
    val projection_conversion_X = udf {(Vkpv: Int) =>
      val Xt: Double = sin(2*Pi*(Vkpv.toFloat/7))
      Xt
    }
    //udf to project the DOW value on unitcircle in respect of Y
    val projection_conversion_Y = udf {(Vkpv: Int) =>
      val Yt: Double = cos(2*Pi*(Vkpv.toFloat/7))
      Yt
    }
    //cleans the data
    val clean_xyz_data: DataFrame = clean_nullsXYDOW(xyDOW_data.withColumn("Vkpv", DOW_to_int_conversion(xyDOW_data("Vkpv"))))
    //projects the data to an unitcircle
    val projected_xyz_data: DataFrame = clean_xyz_data.withColumn("Xt", projection_conversion_X(clean_xyz_data("Vkpv")))
                                                      .withColumn("Yt", projection_conversion_Y(clean_xyz_data("Vkpv")))
    // K-means functionality
    val vectorAssembler = new VectorAssembler()
                              .setInputCols(Array("X", "Y", "Xt", "Yt"))
                              .setOutputCol("features")
    val transformationPipeline = new Pipeline().setStages(Array(vectorAssembler))
    val pipeLine = transformationPipeline.fit(projected_xyz_data)
    val transformedTraining = pipeLine.transform(projected_xyz_data) 
    val clustercount = 7
      for (clustercount <- 7 to 30) {  
        val kmModel = fit_kmModel(clustercount*10, transformedTraining)
        val WSSSE = kmModel.computeCost(transformedTraining)
        println(clustercount*10 + "," + WSSSE)
        
      } 
  }
  
  /**
   * Clusters the X,Y,dayofweek data using dayofweek projection to unitcircle
   */
  def k_means_clustering_dataframe_3d(data: DataFrame){
    val xyDOW_data: DataFrame = data.select("X", "Y", "Vkpv")
    
    //udf to change day of week from "Maanantai, Tiistai,..." to 1-7
    val DOW_to_int_conversion = udf {(Vkpv: String) =>
      val DOW_int = DOW_to_int(Vkpv.toLowerCase)
      DOW_int
    }
    //udf to project the DOW value on unitcircle in respect of X
    val projection_conversion_X = udf {(Vkpv: Int) =>
      val Xt: Double = sin(2*Pi*(Vkpv.toFloat/7))
      Xt
    }
    //udf to project the DOW value on unitcircle in respect of Y
    val projection_conversion_Y = udf {(Vkpv: Int) =>
      val Yt: Double = cos(2*Pi*(Vkpv.toFloat/7))
      Yt
    }
    //cleans the data
    val clean_xyz_data: DataFrame = clean_nullsXYDOW(xyDOW_data.withColumn("Vkpv", DOW_to_int_conversion(xyDOW_data("Vkpv"))))
    //projects the data to an unitcircle
    val projected_xyz_data: DataFrame = clean_xyz_data.withColumn("Xt", projection_conversion_X(clean_xyz_data("Vkpv")))
                                                      .withColumn("Yt", projection_conversion_Y(clean_xyz_data("Vkpv")))
    // K-means functionality
    val vectorAssembler = new VectorAssembler()
                              .setInputCols(Array("X", "Y", "Xt", "Yt"))
                              .setOutputCol("features")
    val transformationPipeline = new Pipeline().setStages(Array(vectorAssembler))
    val pipeLine = transformationPipeline.fit(projected_xyz_data)
    val transformedTraining = pipeLine.transform(projected_xyz_data) 
    val clustercount = 50
    val kmeans = new KMeans().setK(clustercount).setSeed(1L)   
    val kmModel = kmeans.fit(transformedTraining)
    
    //K-means result are put in an array
    val centersArr = kmModel.clusterCenters.map(center => center.toArray)
    
    // four dimesnional cluster centers are converted back to 3 dimensional space
    for (i<-0 until centersArr.length){
      val newline = Xt_Yt_to_DOW(centersArr(i))
      centersArr(i) = newline
    }
    val headers = Array("X", "Y")
    //Write the result to cvs
    CSV_writer.writeCsv("task2", headers, centersArr)

    
  }
  
  //cleans nulls from two dimensional data (X,Y)
  def clean_nullsXY(df: DataFrame) : DataFrame = df.where("X is not null and Y is not null")
  
  //cleans nulls from three dimensional data (X,Y,dayofweek)
  def clean_nullsXYDOW(df: DataFrame) : DataFrame = df.where("X is not null and Y is not null and Vkpv > 0")
  
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
  * transforms the X,Y,Xt,Yt data back to 3 dimensional X,Y,DOW data
  * returns an array which contains this data
  */
  def Xt_Yt_to_DOW(center: Array[Double]) : Array[Double] = {
    //take the Xt and Yt values from array
    val Xt = center(2)
    val Yt = center(3)
    //calculate the day
    var day = atan(Yt/Xt)/2/Pi*7
    
    val x = sin(2*Pi*(Xt/7))
    val y = cos(2*Pi*(Yt/7))
    //if the new x and y dont match with the previous, the point is on the left side of the unitcirle
    // in this case, Pi is added so the angle is correctly calculated
    if (x != Xt || y != Yt) day = (atan(Yt/Xt)+Pi)/2/Pi*7 else day = day
    //for negative angles
    if (day < 0) day += 7 else day = day
    
    center(2) = day
    
    //will not return projected Xt and Yt values
    val center3d = center.take(3)
    return center3d
  }
}