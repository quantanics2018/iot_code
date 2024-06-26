-- phpMyAdmin SQL Dump
-- version 4.9.5deb2
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: May 20, 2024 at 06:11 PM
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
-- Table structure for table `large_kit`
--

CREATE TABLE `large_kit` (
  `rno` int(100) NOT NULL,
  `temperature` int(11) NOT NULL,
  `humidity` int(11) NOT NULL,
  `distance1` int(11) NOT NULL,
  `distance2` int(11) NOT NULL,
  `smoke_val` int(11) NOT NULL,
  `ldr_val` int(11) NOT NULL,
  `soil_moisture` int(11) NOT NULL,
  `raindrop` int(11) NOT NULL,
  `flowrate` int(11) NOT NULL,
  `last_updated_on` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `large_kit`
--

INSERT INTO `large_kit` (`rno`, `temperature`, `humidity`, `distance1`, `distance2`, `smoke_val`, `ldr_val`, `soil_moisture`, `raindrop`, `flowrate`, `last_updated_on`) VALUES
(1, 30, 74, 366, 165, 4, 0, 0, 0, 0, '2024-05-20 12:36:14'),
(2, 30, 74, 1188, 165, 6, 0, 0, 0, 0, '2024-05-20 12:36:20'),
(3, 30, 74, 143, 165, 0, 0, 0, 0, 0, '2024-05-20 12:36:24'),
(4, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:36:28'),
(5, 30, 74, 143, 166, 0, 0, 0, 0, 0, '2024-05-20 12:36:35'),
(6, 30, 74, 144, 165, 0, 0, 0, 0, 0, '2024-05-20 12:36:38'),
(7, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:36:42'),
(8, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:36:49'),
(9, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:36:52'),
(10, 30, 74, 142, 166, 17, 0, 0, 0, 0, '2024-05-20 12:36:59'),
(11, 30, 74, 1188, 165, 1, 0, 0, 0, 0, '2024-05-20 12:37:03'),
(12, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:37:06'),
(13, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:37:14'),
(14, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:37:17'),
(15, 30, 74, 366, 165, 3, 0, 0, 0, 0, '2024-05-20 12:37:20'),
(16, 30, 74, 1188, 167, 0, 0, 0, 0, 0, '2024-05-20 12:37:27'),
(17, 30, 74, 143, 167, 0, 0, 0, 0, 0, '2024-05-20 12:37:31'),
(18, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:37:34'),
(19, 30, 74, 1188, 165, 12, 0, 0, 0, 0, '2024-05-20 12:37:41'),
(20, 30, 74, 142, 166, 0, 0, 0, 0, 0, '2024-05-20 12:37:45'),
(21, 30, 74, 1188, 167, 0, 0, 0, 0, 0, '2024-05-20 12:37:52'),
(22, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:37:55'),
(23, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:37:59'),
(24, 30, 74, 1188, 165, 1, 0, 0, 0, 0, '2024-05-20 12:38:06'),
(25, 30, 74, 1188, 165, 10, 0, 0, 0, 0, '2024-05-20 12:38:09'),
(26, 30, 74, 1188, 166, 4, 0, 0, 0, 0, '2024-05-20 12:38:13'),
(27, 30, 74, 1188, 165, 6, 0, 0, 0, 0, '2024-05-20 12:38:20'),
(28, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:38:24'),
(29, 30, 74, 1188, 175, 0, 0, 0, 0, 0, '2024-05-20 12:38:28'),
(30, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:38:35'),
(31, 30, 74, 144, 165, 0, 0, 0, 0, 0, '2024-05-20 12:38:38'),
(32, 30, 74, 144, 165, 2, 0, 0, 0, 0, '2024-05-20 12:38:45'),
(33, 30, 74, 1188, 165, 4, 0, 0, 0, 0, '2024-05-20 12:38:48'),
(34, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:38:52'),
(35, 30, 74, 1188, 166, 2, 0, 0, 0, 0, '2024-05-20 12:38:59'),
(36, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:39:02'),
(37, 30, 74, 1188, 165, 13, 0, 0, 0, 0, '2024-05-20 12:39:06'),
(38, 30, 74, 1188, 166, 12, 0, 0, 0, 0, '2024-05-20 12:39:13'),
(39, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:39:17'),
(40, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:39:27'),
(41, 30, 74, 143, 165, 0, 0, 0, 0, 0, '2024-05-20 12:39:30'),
(42, 30, 74, 144, 166, 1, 0, 0, 0, 0, '2024-05-20 12:39:38'),
(43, 30, 74, 366, 165, 6, 0, 0, 0, 0, '2024-05-20 12:39:41'),
(44, 30, 74, 1188, 166, 2, 0, 0, 0, 0, '2024-05-20 12:39:45'),
(45, 30, 74, 1188, 167, 5, 0, 0, 0, 0, '2024-05-20 12:39:52'),
(46, 30, 74, 1188, 166, 5, 0, 0, 0, 0, '2024-05-20 12:39:56'),
(47, 30, 74, 1188, 165, 2, 0, 0, 0, 0, '2024-05-20 12:39:59'),
(48, 30, 74, 1188, 166, 1, 0, 0, 0, 0, '2024-05-20 12:40:06'),
(49, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:40:09'),
(50, 30, 74, 142, 165, 16, 0, 0, 0, 0, '2024-05-20 12:40:13'),
(51, 30, 74, 1188, 165, 10, 0, 0, 0, 0, '2024-05-20 12:40:20'),
(52, 30, 74, 1188, 165, 4, 0, 0, 0, 0, '2024-05-20 12:40:23'),
(53, 30, 74, 1188, 165, 2, 0, 0, 0, 0, '2024-05-20 12:40:31'),
(54, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:40:34'),
(55, 30, 74, 142, 166, 4, 0, 0, 0, 0, '2024-05-20 12:40:38'),
(56, 30, 74, 1188, 165, 17, 0, 0, 0, 0, '2024-05-20 12:40:45'),
(57, 30, 74, 1188, 167, 0, 0, 0, 0, 0, '2024-05-20 12:40:48'),
(58, 30, 74, 144, 165, 0, 0, 0, 0, 0, '2024-05-20 12:40:51'),
(59, 30, 74, 1188, 166, 1, 0, 0, 0, 0, '2024-05-20 12:40:59'),
(60, 30, 74, 143, 165, 4, 0, 0, 0, 0, '2024-05-20 12:41:02'),
(61, 30, 74, 1188, 166, 6, 0, 0, 0, 0, '2024-05-20 12:41:06'),
(62, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:41:13'),
(63, 30, 74, 1188, 165, 1, 0, 0, 0, 0, '2024-05-20 12:41:16'),
(64, 30, 74, 1188, 165, 0, 0, 0, 0, 0, '2024-05-20 12:41:23'),
(65, 30, 74, 1188, 165, 6, 0, 0, 0, 0, '2024-05-20 12:41:27'),
(66, 30, 74, 1188, 166, 6, 0, 0, 0, 0, '2024-05-20 12:41:30'),
(67, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:41:37'),
(68, 30, 74, 1188, 166, 0, 0, 0, 0, 0, '2024-05-20 12:41:41'),
(69, 30, 74, 1188, 165, 5, 0, 0, 0, 0, '2024-05-20 12:41:44');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `large_kit`
--
ALTER TABLE `large_kit`
  ADD PRIMARY KEY (`rno`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `large_kit`
--
ALTER TABLE `large_kit`
  MODIFY `rno` int(100) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=70;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
