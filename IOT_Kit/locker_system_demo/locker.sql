-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Nov 15, 2024 at 01:31 PM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `iotkit`
--

-- --------------------------------------------------------

--
-- Table structure for table `locker`
--

CREATE TABLE `locker` (
  `sno` int(11) NOT NULL,
  `distance` int(11) NOT NULL,
  `ldr` int(11) NOT NULL,
  `smoke` int(11) NOT NULL,
  `updatedtimestamp` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `locker`
--

INSERT INTO `locker` (`sno`, `distance`, `ldr`, `smoke`, `updatedtimestamp`) VALUES
(1, 56, 4095, 0, '2024-11-15 12:23:26'),
(2, 56, 4095, 0, '2024-11-15 12:23:30'),
(3, 53, 4095, 0, '2024-11-15 12:23:32'),
(4, 60, 4095, 0, '2024-11-15 12:23:49'),
(5, 53, 4095, 0, '2024-11-15 12:24:10'),
(6, 24, 4095, 0, '2024-11-15 12:24:12'),
(7, 56, 4095, 0, '2024-11-15 12:24:17'),
(8, 57, 4095, 0, '2024-11-15 12:24:19'),
(9, 53, 4095, 0, '2024-11-15 12:24:23'),
(10, 52, 4095, 0, '2024-11-15 12:24:25'),
(11, 53, 4095, 0, '2024-11-15 12:24:27'),
(12, 36, 4095, 0, '2024-11-15 12:24:36'),
(13, 26, 4095, 0, '2024-11-15 12:24:48'),
(14, 12, 4095, 0, '2024-11-15 12:25:51'),
(15, 51, 4095, 0, '2024-11-15 12:25:53'),
(16, 38, 4095, 0, '2024-11-15 12:26:00'),
(17, 24, 4095, 0, '2024-11-15 12:26:07'),
(18, 27, 4095, 0, '2024-11-15 12:26:22'),
(19, 23, 4095, 0, '2024-11-15 12:26:32'),
(20, 23, 4095, 0, '2024-11-15 12:26:46'),
(21, 42, 4095, 0, '2024-11-15 12:27:00'),
(22, 36, 4095, 0, '2024-11-15 12:27:06'),
(23, 53, 4095, 0, '2024-11-15 12:27:11'),
(24, 53, 4095, 0, '2024-11-15 12:27:15'),
(25, 36, 4095, 0, '2024-11-15 12:28:46'),
(26, 23, 4095, 0, '2024-11-15 12:28:47'),
(27, 56, 4095, 0, '2024-11-15 12:28:52'),
(28, 55, 4095, 0, '2024-11-15 12:28:53'),
(29, 55, 4095, 0, '2024-11-15 12:29:04'),
(30, 56, 4095, 0, '2024-11-15 12:29:20'),
(31, 29, 4095, 0, '2024-11-15 12:29:38'),
(32, 1197, 4095, 0, '2024-11-15 12:30:31'),
(33, 52, 4095, 0, '2024-11-15 12:30:34'),
(34, 53, 4095, 0, '2024-11-15 12:30:40'),
(35, 54, 4095, 0, '2024-11-15 12:31:10'),
(36, 53, 4095, 0, '2024-11-15 12:31:14'),
(37, 42, 4095, 0, '2024-11-15 12:31:17'),
(38, 42, 4095, 0, '2024-11-15 12:31:22'),
(39, 42, 4095, 0, '2024-11-15 12:31:24'),
(40, 39, 4095, 0, '2024-11-15 12:31:35'),
(41, 52, 4095, 0, '2024-11-15 12:31:37'),
(42, 60, 4095, 0, '2024-11-15 12:31:41'),
(43, 42, 4095, 0, '2024-11-15 12:31:50');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `locker`
--
ALTER TABLE `locker`
  ADD PRIMARY KEY (`sno`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `locker`
--
ALTER TABLE `locker`
  MODIFY `sno` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=44;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
