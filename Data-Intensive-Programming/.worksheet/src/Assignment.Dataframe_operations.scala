package Assignment

import org.apache.spark.SparkConf
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


object Dataframe_operations {;import org.scalaide.worksheet.runtime.library.WorksheetSupport._; def main(args: Array[String])=$execute{;$skip(1957); 
    
  def k_means_clustering_dataframe_2d(data: DataFrame){
    val xy_data: DataFrame = data.select("X", "Y")
    val clean_xy_data = clean_nullsXY(xy_data)
    //xy_data.show()
    val vectorAssembler = new VectorAssembler()
                              .setInputCols(Array("X", "Y"))
                              .setOutputCol("features")
    val transformationPipeline = new Pipeline().setStages(Array(vectorAssembler))
    val pipeLine = transformationPipeline.fit(clean_xy_data)
    val transformedTraining = pipeLine.transform(clean_xy_data)
    val kmeans = new KMeans().setK(300).setSeed(1L)
    
    val kmModel = kmeans.fit(transformedTraining)
    //kmModel.summary.predictions.show
    kmModel.clusterCenters.foreach(println)
    
    
  };System.out.println("""k_means_clustering_dataframe_2d: (data: org.apache.spark.sql.DataFrame)Unit""");$skip(185); 
  def k_means_clustering_datafram_3d(data: DataFrame){
    val xyDOW_data: DataFrame = data.select("X", "Y", "Vkpv")
    val clean_xyDOW_data = clean_nullsXYDOW(xyDOW_data)
    
  
  };System.out.println("""k_means_clustering_datafram_3d: (data: org.apache.spark.sql.DataFrame)Unit""");$skip(93); 
  def clean_nullsXY(df: DataFrame) : DataFrame = df.where("X is not null and Y is not null");System.out.println("""clean_nullsXY: (df: org.apache.spark.sql.DataFrame)org.apache.spark.sql.DataFrame""");$skip(117); 
  def clean_nullsXYDOW(df: DataFrame) : DataFrame = df.where("X is not null and Y is not null and Vkpv is not null");System.out.println("""clean_nullsXYDOW: (df: org.apache.spark.sql.DataFrame)org.apache.spark.sql.DataFrame""")}
}
