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
-- Table structure for table `small_kit`
--

CREATE TABLE `small_kit` (
  `rno` int(100) NOT NULL,
  `humidity` bigint(100) NOT NULL,
  `temperature` bigint(100) NOT NULL,
  `distance1` bigint(100) NOT NULL,
  `distance2` bigint(100) NOT NULL,
  `smoke_val` int(11) NOT NULL,
  `ir_val` int(11) NOT NULL,
  `last_updated_on` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `small_kit`
--

INSERT INTO `small_kit` (`rno`, `humidity`, `temperature`, `distance1`, `distance2`, `smoke_val`, `ir_val`, `last_updated_on`) VALUES
(1, 95, 30, 138, 1193, 96, 1, '2024-05-20 11:29:25'),
(2, 95, 30, 138, 133, 97, 1, '2024-05-20 11:29:30'),
(3, 95, 30, 138, 134, 96, 1, '2024-05-20 11:29:36'),
(4, 95, 30, 139, 133, 96, 1, '2024-05-20 11:29:42'),
(5, 95, 30, 139, 1192, 96, 1, '2024-05-20 11:29:48'),
(6, 95, 30, 139, 133, 94, 1, '2024-05-20 11:29:53'),
(7, 95, 30, 138, 1192, 96, 1, '2024-05-20 11:29:59'),
(8, 95, 30, 138, 1193, 96, 1, '2024-05-20 11:30:05'),
(9, 95, 30, 79, 134, 96, 1, '2024-05-20 11:30:17'),
(10, 95, 30, 138, 132, 96, 1, '2024-05-20 11:30:22'),
(11, 95, 30, 139, 135, 97, 1, '2024-05-20 11:30:28'),
(12, 95, 30, 139, 134, 96, 1, '2024-05-20 11:30:34'),
(13, 95, 30, 138, 134, 96, 1, '2024-05-20 11:30:39'),
(14, 95, 30, 139, 136, 94, 1, '2024-05-20 11:30:45'),
(15, 95, 30, 139, 1193, 94, 1, '2024-05-20 11:30:51'),
(16, 95, 30, 138, 134, 94, 1, '2024-05-20 11:30:57'),
(17, 95, 30, 139, 134, 92, 1, '2024-05-20 11:31:03'),
(18, 95, 30, 146, 138, 93, 1, '2024-05-20 11:31:08'),
(19, 95, 30, 138, 133, 94, 1, '2024-05-20 11:31:14'),
(20, 95, 30, 138, 134, 94, 1, '2024-05-20 11:31:20'),
(21, 95, 30, 138, 133, 96, 1, '2024-05-20 11:31:25'),
(22, 95, 30, 138, 1193, 94, 1, '2024-05-20 11:31:31'),
(23, 95, 30, 139, 133, 94, 1, '2024-05-20 11:31:37'),
(24, 95, 30, 138, 134, 94, 1, '2024-05-20 11:31:43'),
(25, 95, 30, 138, 133, 94, 1, '2024-05-20 11:31:48'),
(26, 95, 30, 138, 133, 94, 1, '2024-05-20 11:31:54');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `small_kit`
--
ALTER TABLE `small_kit`
  ADD PRIMARY KEY (`rno`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `small_kit`
--
ALTER TABLE `small_kit`
  MODIFY `rno` int(100) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=27;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
