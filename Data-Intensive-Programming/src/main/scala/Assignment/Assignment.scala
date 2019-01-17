package Assignment

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.sql.functions.{ window, column, desc, col }

import org.apache.spark.ml.clustering.KMeans
import org.apache.spark.ml.feature.VectorAssembler
import org.apache.spark.ml.Pipeline


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

import org.apache.spark.sql.functions.explode
import org.apache.spark.sql.functions.array
import org.apache.spark.sql.SparkSession

import com.databricks.spark.xml._
import org.apache.spark.storage.StorageLevel
import org.apache.spark.streaming.{ Seconds, StreamingContext }

import java.lang.Thread
import org.apache.spark.rdd.RDD

import org.apache.log4j.Logger
import org.apache.log4j.Level

import Assignment.DataFrame_operations
import Assignment.RDD_operations


object main extends App {

  // Suppress the log messages:
  Logger.getLogger("org").setLevel(Level.OFF)

  val spark = SparkSession.builder()
    .appName("Assignment")
    .config("spark.driver.host", "localhost")
    .master("local")
    .getOrCreate()

  
  // Here we can implement schema for optimization reasons
    
  //Reads the data to dataframe
  val accidentDataFrame: DataFrame = spark.read
                                          .format("csv")
                                          .option("delimiter", ";")
                                          .option("header", "true")
                                          .option("inferSchema", "true")
                                          .load("data/tieliikenneonnettomuudet_2015_onnettomuus.csv")
                                          
  //Reads the data to RDD
  val rdd = spark.sparkContext.textFile("data/tieliikenneonnettomuudet_2015_onnettomuus.csv")

  if (args.length == 0) {
    println("No arguments given!")
  } 
  else if (args.length < 2) {
    println("Not enough arguments given, try to add e.g. '-task 1'")
    println("Arguments were:")
    args.foreach(println)
  } 
  else if (args(0) == "-task") {
    val taskNumber = args(1)
    runTask(taskNumber)
  }

  def runTask(taskNumber: String) {
    taskNumber match {
      case "1" => DataFrame_operations.k_means_clustering_dataframe_2d(accidentDataFrame)
      case "2" => DataFrame_operations.k_means_clustering_dataframe_3d(accidentDataFrame)
      case "3" => println("Implementation can be found ASSIGNMENT_DESCRIPTION.pdf")
      case "4" => DataFrame_operations.count_elbow_values_2d(accidentDataFrame)
      case "5" => RDD_operations.k_means_clustering_rdd_3d(rdd)
      case "6" => println("Not implemented!")
      case _   => println("Invalid task number: " + args(1))
    }
  }

}
