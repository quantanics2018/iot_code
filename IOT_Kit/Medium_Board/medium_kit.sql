-- phpMyAdmin SQL Dump
-- version 4.9.5deb2
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: May 20, 2024 at 06:36 PM
-- Server version: 10.3.39-MariaDB-0ubuntu0.20.04.2
-- PHP Version: 7.4.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `iot_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `medium_kit`
--

CREATE TABLE `medium_kit` (
  `rno` int(11) NOT NULL,
  `distance` int(11) NOT NULL,
  `soil_moisture` int(11) NOT NULL,
  `last_updated_on` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `medium_kit`
--

INSERT INTO `medium_kit` (`rno`, `distance`, `soil_moisture`, `last_updated_on`) VALUES
(1, 0, 1, '2024-05-20 13:04:57'),
(2, 0, 1, '2024-05-20 13:05:04'),
(3, 0, 1, '2024-05-20 13:05:11'),
(4, 0, 1, '2024-05-20 13:05:24'),
(5, 0, 1, '2024-05-20 13:05:38'),
(6, 0, 1, '2024-05-20 13:05:45'),
(7, 0, 1, '2024-05-20 13:05:52'),
(8, 0, 1, '2024-05-20 13:05:59'),
(9, 0, 1, '2024-05-20 13:06:05'),
(10, 0, 1, '2024-05-20 13:06:12');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `medium_kit`
--
ALTER TABLE `medium_kit`
  ADD PRIMARY KEY (`rno`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `medium_kit`
--
ALTER TABLE `medium_kit`
  MODIFY `rno` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
