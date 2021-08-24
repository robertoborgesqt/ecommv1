/*
SQLyog Ultimate v12.09 (64 bit)
MySQL - 5.7.35 : Database - ecomm
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`ecomm` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `ecomm`;

/*Table structure for table `cliente` */

DROP TABLE IF EXISTS `cliente`;

CREATE TABLE `cliente` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) DEFAULT NULL,
  `telefone` varchar(32) DEFAULT NULL,
  `sobrenome` varchar(32) DEFAULT NULL,
  `email` varchar(32) DEFAULT NULL,
  `sexo` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

/*Data for the table `cliente` */

insert  into `cliente`(`id`,`nome`,`telefone`,`sobrenome`,`email`,`sexo`) values (1,'Roberto Borges dos Santos','11-987346486',NULL,NULL,NULL);

/*Table structure for table `endereco` */

DROP TABLE IF EXISTS `endereco`;

CREATE TABLE `endereco` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tipo` int(11) DEFAULT NULL,
  `id_cliente` int(11) DEFAULT NULL,
  `logradouro` varchar(255) DEFAULT NULL,
  `complemento` varchar(255) DEFAULT NULL,
  `cidade` varchar(64) DEFAULT NULL,
  `uf` varchar(2) DEFAULT NULL,
  `cep` varchar(12) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

/*Data for the table `endereco` */

insert  into `endereco`(`id`,`tipo`,`id_cliente`,`logradouro`,`complemento`,`cidade`,`uf`,`cep`) values (1,1,1,'Av Barber Greene, 729','Casa','Guarulhos','SP','07120260'),(2,2,1,'Av Paulista, 2239','10 Andar Sala 4','São Paulo','SP','02304450'),(3,3,1,'Av Angelica, 1334','Apto 34','São Paulo','SP','02344457');

/*Table structure for table `pagamento` */

DROP TABLE IF EXISTS `pagamento`;

CREATE TABLE `pagamento` (
  `seq` bigint(20) NOT NULL AUTO_INCREMENT,
  `id_cliente` int(11) DEFAULT NULL,
  `dt_compra` datetime DEFAULT CURRENT_TIMESTAMP,
  `valor` float NOT NULL,
  `tipo` int(11) DEFAULT NULL,
  PRIMARY KEY (`seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `pagamento` */

/*Table structure for table `produto` */

DROP TABLE IF EXISTS `produto`;

CREATE TABLE `produto` (
  `id` int(11) NOT NULL,
  `descr` varchar(255) DEFAULT NULL,
  `preco` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `produto` */

insert  into `produto`(`id`,`descr`,`preco`) values (1,'Banana','1,20'),(2,'Maça','2,20'),(3,'Melancia','4,00'),(4,'Limão','1,00'),(5,'Pera','3.00'),(6,'Laranja','3.00'),(7,'Uva','6,00'),(8,'Abacaxi','5,00'),(9,'Caju','5,00');

/*Table structure for table `tipo_endereco` */

DROP TABLE IF EXISTS `tipo_endereco`;

CREATE TABLE `tipo_endereco` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descr` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

/*Data for the table `tipo_endereco` */

insert  into `tipo_endereco`(`id`,`descr`) values (1,'Residencial'),(2,'Comercial'),(3,'Outro');

/*Table structure for table `tipo_pagamento` */

DROP TABLE IF EXISTS `tipo_pagamento`;

CREATE TABLE `tipo_pagamento` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `descr` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

/*Data for the table `tipo_pagamento` */

insert  into `tipo_pagamento`(`id`,`descr`) values (1,'Visa Credito'),(2,'MasterCard Credito'),(3,'Dinheiro'),(4,'Debito');

/*Table structure for table `transportadora` */

DROP TABLE IF EXISTS `transportadora`;

CREATE TABLE `transportadora` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `transportadora` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
